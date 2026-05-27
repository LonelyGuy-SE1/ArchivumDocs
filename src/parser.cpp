#include <algorithm>
#include <archivum/parser.hpp>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>

extern "C" {
#include <tree_sitter/api.h>
}

namespace archivum {

namespace {

uint64_t stable_hash(std::string_view value) {
    uint64_t hash = 14695981039346656037ull;
    for (unsigned char c : value) {
        hash ^= c;
        hash *= 1099511628211ull;
    }
    return hash;
}

std::string text_for_node(TSNode node, const std::string& source_code) {
    if (ts_node_is_null(node)) {
        return {};
    }
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    if (start > end || end > source_code.size()) {
        return {};
    }
    return source_code.substr(start, end - start);
}

std::string normalize_space(std::string value) {
    std::string result;
    result.reserve(value.size());
    bool pending_space = false;

    for (unsigned char c : value) {
        if (std::isspace(c)) {
            pending_space = !result.empty();
            continue;
        }
        if (pending_space && c != ';' && c != ',' && c != ')' && c != '(' && c != '{') {
            result.push_back(' ');
        }
        pending_space = false;
        result.push_back(static_cast<char>(c));
    }

    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.back()))) {
        result.pop_back();
    }

    return result;
}

bool is_identifier_node(std::string_view type) {
    return type == "identifier" || type == "field_identifier" || type == "type_identifier" ||
           type == "namespace_identifier" || type == "qualified_identifier" || type == "scoped_identifier" ||
           type == "destructor_name" || type == "operator_name" || type == "template_type" ||
           type == "template_function";
}

std::string simple_symbol(std::string value) {
    size_t template_start = value.find('<');
    if (template_start != std::string::npos) {
        value = value.substr(0, template_start);
    }
    size_t scope = value.rfind("::");
    if (scope != std::string::npos) {
        value = value.substr(scope + 2);
    }
    while (!value.empty() && !std::isalnum(static_cast<unsigned char>(value.front())) && value.front() != '_' &&
           value.front() != '~') {
        value.erase(value.begin());
    }
    while (!value.empty() && !std::isalnum(static_cast<unsigned char>(value.back())) && value.back() != '_') {
        value.pop_back();
    }
    return value;
}

std::string first_named_descendant(TSNode node, const std::string& source_code) {
    if (ts_node_is_null(node)) {
        return {};
    }

    std::string type = ts_node_type(node);
    if (is_identifier_node(type)) {
        return text_for_node(node, source_code);
    }

    static constexpr const char* fields[] = {"declarator", "name"};
    for (const char* field : fields) {
        TSNode child =
            ts_node_child_by_field_name(node, field, static_cast<uint32_t>(std::char_traits<char>::length(field)));
        std::string value = first_named_descendant(child, source_code);
        if (!value.empty()) {
            return value;
        }
    }

    uint32_t child_count = ts_node_named_child_count(node);
    for (uint32_t index = 0; index < child_count; ++index) {
        std::string value = first_named_descendant(ts_node_named_child(node, index), source_code);
        if (!value.empty()) {
            return value;
        }
    }

    return {};
}

std::string structural_name(TSNode node, const std::string& source_code) {
    std::string type = ts_node_type(node);
    if (type == "class_specifier" || type == "struct_specifier") {
        TSNode name = ts_node_child_by_field_name(node, "name", 4);
        return text_for_node(name, source_code);
    }

    TSNode declarator = ts_node_child_by_field_name(node, "declarator", 10);
    return first_named_descendant(declarator, source_code);
}

bool has_class_ancestor(TSNode node) {
    TSNode parent = ts_node_parent(node);
    while (!ts_node_is_null(parent)) {
        std::string type = ts_node_type(parent);
        if (type == "class_specifier" || type == "struct_specifier") {
            return true;
        }
        parent = ts_node_parent(parent);
    }
    return false;
}

std::string interface_text(TSNode node, const std::string& source_code) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);

    TSNode body = ts_node_child_by_field_name(node, "body", 4);
    if (!ts_node_is_null(body)) {
        end = ts_node_start_byte(body);
    } else {
        size_t brace = source_code.find('{', start);
        if (brace != std::string::npos && brace < end) {
            end = static_cast<uint32_t>(brace);
        }
    }

    if (start > end || end > source_code.size()) {
        return {};
    }

    return normalize_space(source_code.substr(start, end - start));
}

void collect_references(TSNode node, const std::string& source_code, std::set<std::string>& references) {
    std::string type = ts_node_type(node);
    if (is_identifier_node(type)) {
        std::string value = simple_symbol(text_for_node(node, source_code));
        if (!value.empty()) {
            references.insert(value);
        }
    }

    uint32_t child_count = ts_node_named_child_count(node);
    for (uint32_t index = 0; index < child_count; ++index) {
        collect_references(ts_node_named_child(node, index), source_code, references);
    }
}

}  // namespace

ASTParser::ASTParser() {
    parser = ts_parser_new();
    if (parser == nullptr || !ts_parser_set_language(parser, tree_sitter_cpp())) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Failed to initialize the C++ parser.");
    }
}

ASTParser::~ASTParser() {
    if (parser != nullptr) {
        ts_parser_delete(parser);
    }
}

uint64_t ASTParser::generate_node_id(const std::string& filepath, const std::string& symbol_name) const {
    std::string composite_key = filepath + "::" + symbol_name;
    return stable_hash(composite_key);
}

std::vector<Node> ASTParser::parse_file(const std::string& filepath) {
    std::vector<Node> extracted_nodes;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[ArchivumDocs] ERROR: Could not open file: " << filepath << std::endl;
        return extracted_nodes;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source_code = buffer.str();

    if (source_code.size() > std::numeric_limits<uint32_t>::max()) {
        std::cerr << "[ArchivumDocs] ERROR: File is too large for tree-sitter: " << filepath << std::endl;
        return extracted_nodes;
    }

    TSTree* tree =
        ts_parser_parse_string(parser, nullptr, source_code.c_str(), static_cast<uint32_t>(source_code.size()));

    if (!tree) {
        std::cerr << "[ArchivumDocs] FATAL: Tree-sitter failed to parse file." << std::endl;
        return extracted_nodes;
    }

    TSNode root_node = ts_tree_root_node(tree);

    extract_structural_nodes(root_node, extracted_nodes, filepath, source_code);

    ts_tree_delete(tree);

    return extracted_nodes;
}

void ASTParser::extract_structural_nodes(TSNode root_node, std::vector<Node>& extracted_nodes,
                                         const std::string& filepath, const std::string& source_code) {
    TSTreeCursor cursor = ts_tree_cursor_new(root_node);
    bool reached_root = false;

    while (!reached_root) {
        TSNode current_node = ts_tree_cursor_current_node(&cursor);
        std::string type = ts_node_type(current_node);

        if (type == "function_definition" || type == "class_specifier" || type == "struct_specifier") {
            Node current;
            current.file_path = filepath;

            TSPoint start_point = ts_node_start_point(current_node);
            TSPoint end_point = ts_node_end_point(current_node);
            current.start_line = start_point.row + 1;
            current.end_line = end_point.row + 1;

            if (type == "function_definition") {
                current.type = has_class_ancestor(current_node) ? NodeType::METHOD : NodeType::FUNCTION;
            } else if (type == "class_specifier") {
                current.type = NodeType::CLASS;
            } else {
                current.type = NodeType::STRUCT;
            }

            current.name = structural_name(current_node, source_code);
            if (current.name.empty()) {
                current.name = "unnamed_" + type + "_" + std::to_string(current.start_line);
            }

            std::set<std::string> references;
            collect_references(current_node, source_code, references);
            references.erase(simple_symbol(current.name));

            uint32_t start_byte = ts_node_start_byte(current_node);
            uint32_t end_byte = ts_node_end_byte(current_node);
            std::string source_slice;
            if (start_byte <= end_byte && end_byte <= source_code.size()) {
                source_slice = source_code.substr(start_byte, end_byte - start_byte);
            }

            current.signature = interface_text(current_node, source_code);
            current.id = generate_node_id(filepath, current.name);
            current.interface_hash = stable_hash(current.signature);
            current.source_hash = stable_hash(source_slice);
            current.references.assign(references.begin(), references.end());

            extracted_nodes.push_back(current);
        }

        if (ts_tree_cursor_goto_first_child(&cursor))
            continue;
        if (ts_tree_cursor_goto_next_sibling(&cursor))
            continue;

        bool retracing = true;
        while (retracing) {
            if (!ts_tree_cursor_goto_parent(&cursor)) {
                reached_root = true;
                break;
            }
            if (ts_tree_cursor_goto_next_sibling(&cursor)) {
                retracing = false;
            }
        }
    }

    ts_tree_cursor_delete(&cursor);
}
}  // namespace archivum
