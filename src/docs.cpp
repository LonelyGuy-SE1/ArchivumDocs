#include <algorithm>
#include <archivum/docs.hpp>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>

namespace archivum {

namespace {

std::string type_name(NodeType type) {
    switch (type) {
        case NodeType::FUNCTION:
            return "Function";
        case NodeType::CLASS:
            return "Class";
        case NodeType::STRUCT:
            return "Struct";
        case NodeType::METHOD:
            return "Method";
        default:
            return "Symbol";
    }
}

std::string sanitize(std::string value) {
    for (char& c : value) {
        bool keep = std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_';
        if (!keep) {
            c = '-';
        }
    }

    value.erase(
        std::unique(value.begin(), value.end(), [](char left, char right) { return left == '-' && right == '-'; }),
        value.end());

    while (!value.empty() && value.front() == '-') {
        value.erase(value.begin());
    }
    while (!value.empty() && value.back() == '-') {
        value.pop_back();
    }
    return value.empty() ? "symbol" : value;
}

std::string short_sha(const std::string& sha) { return sha.size() > 8 ? sha.substr(0, 8) : sha; }

std::string relative_link(const std::filesystem::path& from, const std::filesystem::path& to) {
    return std::filesystem::relative(to, from.parent_path()).generic_string();
}

std::string symbol_filename(const Node& node) {
    std::ostringstream out;
    out << sanitize(node.file_path) << "-" << sanitize(node.name) << "-" << std::hex << node.id << ".md";
    return out.str();
}

std::string read_slice(const std::filesystem::path& root, const Node& node, size_t limit) {
    std::ifstream file(root / node.file_path);
    if (!file.is_open()) {
        return {};
    }

    std::ostringstream out;
    std::string line;
    uint32_t line_number = 0;
    size_t written = 0;

    while (std::getline(file, line)) {
        ++line_number;
        if (line_number < node.start_line) {
            continue;
        }
        if (line_number > node.end_line || written >= limit) {
            break;
        }

        size_t remaining = limit - written;
        if (line.size() + 1 > remaining) {
            out << line.substr(0, remaining);
            written = limit;
        } else {
            out << line << "\n";
            written += line.size() + 1;
        }
    }

    return out.str();
}

bool write_if_changed(const std::filesystem::path& path, const std::string& content) {
    std::ifstream existing(path, std::ios::binary);
    if (existing.is_open()) {
        std::ostringstream buffer;
        buffer << existing.rdbuf();
        if (buffer.str() == content) {
            return false;
        }
    }

    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    out << content;
    return true;
}

std::string render_symbol_page(const Node& node, const std::string& source) {
    std::ostringstream out;
    out << "---\n";
    out << "layout: default\n";
    out << "title: \"Symbol: " << node.name << "\"\n";
    out << "---\n\n";

    out << "# " << node.name << "\n\n";

    out << "| Metadata | Value |\n";
    out << "| :--- | :--- |\n";
    out << "| **Type** | " << type_name(node.type) << " |\n";
    out << "| **Location** | `" << node.file_path << ":" << node.start_line << "-" << node.end_line << "` |\n";
    out << "| **Interface Hash** | `" << node.interface_hash << "` |\n";
    out << "| **Source Hash** | `" << node.source_hash << "` |\n\n";

    if (!node.signature.empty()) {
        out << "## Signature\n\n";
        out << "```cpp\n" << node.signature << "\n```\n\n";
    }

    if (!node.references.empty()) {
        out << "## Dependencies\n\n";
        for (const std::string& reference : node.references) {
            out << "- `" << reference << "`\n";
        }
        out << "\n";
    }

    if (!source.empty()) {
        out << "## Implementation\n\n";
        out << "```cpp\n" << source << "\n```\n";
    }

    return out.str();
}

std::string render_index(const ArchivumConfig& config, const AnalysisReport& report,
                         const std::string& generated_update, const std::filesystem::path& docs_root) {
    std::ostringstream out;
    out << "---\n";
    out << "layout: default\n";
    out << "title: \"ArchivumDocs | Code Intelligence\"\n";
    out << "---\n\n";

    out << "# ArchivumDocs\n\n";

    out << "## System Status\n\n";
    out << "| Statistic | Value |\n";
    out << "| :--- | :--- |\n";
    out << "| **Analysis Range** | `" << short_sha(report.base_sha) << "` &rarr; `" << short_sha(report.head_sha)
        << "` |\n";
    out << "| **Source Files** | " << report.source_file_count << " |\n";
    out << "| **Changed Files** | " << report.changed_file_count << " |\n";
    out << "| **Graph Density** | " << report.graph_node_count << " symbols, " << report.graph_edge_count
        << " edges |\n";
    out << "| **Impact Radius** | " << report.mutated_nodes.size() << " mutated, " << report.context_nodes.size()
        << " context |\n\n";

    if (!generated_update.empty()) {
        out << "## AI Analysis\n\n";
        out << "> " << generated_update << "\n\n";
    }

    out << "## Changes & Impact\n\n";
    if (report.mutated_nodes.empty()) {
        out << "*No structural changes detected in this range.*\n";
    } else {
        out << "| Symbol | Type | Location |\n";
        out << "| :--- | :--- | :--- |\n";
        for (const Node& node : report.mutated_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "| [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") | ";
            out << type_name(node.type) << " | ";
            out << "`" << node.file_path << ":" << node.start_line << "` |\n";
        }
        out << "\n";
    }

    out << "## Downstream Context\n\n";
    if (report.context_nodes.empty()) {
        out << "*No downstream impacts identified.*\n";
    } else {
        out << "| Symbol | Type | Relationship |\n";
        out << "| :--- | :--- | :--- |\n";
        for (const Node& node : report.context_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "| [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") | ";
            out << type_name(node.type) << " | ";
            out << "Contextual dependency |\n";
        }
    }

    return out.str();
}

std::string render_manifest(const AnalysisReport& report) {
    std::ostringstream out;
    out << "{\n";
    out << "  \"base_sha\": \"" << report.base_sha << "\",\n";
    out << "  \"head_sha\": \"" << report.head_sha << "\",\n";
    out << "  \"source_files\": " << report.source_file_count << ",\n";
    out << "  \"changed_files\": " << report.changed_file_count << ",\n";
    out << "  \"graph_nodes\": " << report.graph_node_count << ",\n";
    out << "  \"graph_edges\": " << report.graph_edge_count << ",\n";
    out << "  \"mutated_symbols\": " << report.mutated_nodes.size() << ",\n";
    out << "  \"context_symbols\": " << report.context_nodes.size() << "\n";
    out << "}\n";
    return out.str();
}

}  // namespace

std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report,
                                       const std::filesystem::path& root) {
    std::ostringstream prompt;
    prompt << "You are ArchivumDocs, a documentation maintainer for an elegant and high-integrity C++ codebase. ";
    prompt << "Write a sophisticated and professional Markdown summary of the changes in this range. ";
    prompt << "Focus on architectural implications, changes in API contracts, and how downstream components might be affected. ";
    prompt << "Maintain a refined, technical tone. Do not use code fences for the summary text itself.\n\n";
    prompt << "Range: " << report.base_sha << " -> " << report.head_sha << "\n";
    prompt << "Mutated symbols: " << report.mutated_nodes.size() << "\n";
    prompt << "Context symbols: " << report.context_nodes.size() << "\n\n";

    size_t remaining = config.max_context_bytes;
    size_t emitted = 0;
    for (const Node& node : report.context_nodes) {
        if (emitted >= config.max_symbols || remaining == 0) {
            break;
        }

        std::string source = read_slice(root, node, std::min<size_t>(remaining, 12000));
        size_t consumed = std::min(remaining, source.size());
        remaining -= consumed;
        ++emitted;

        prompt << "### " << node.name << " (" << type_name(node.type) << ")\n";
        prompt << "Location: " << node.file_path << ":" << node.start_line << "-" << node.end_line << "\n";
        if (!node.signature.empty()) {
            prompt << "Signature: " << node.signature << "\n";
        }
        if (!source.empty()) {
            prompt << "Source Snippet:\n" << source << "\n";
        }
        prompt << "\n";
    }

    if (report.context_nodes.size() > emitted) {
        prompt << "Note: Context truncated after " << emitted << " symbols for efficiency.\n";
    }

    return prompt.str();
}

DocumentationWriteResult write_documentation(const ArchivumConfig& config, const AnalysisReport& report,
                                             const std::string& generated_update, const std::filesystem::path& root) {
    DocumentationWriteResult result;
    std::filesystem::path docs_root = root / config.docs_dir;
    std::filesystem::path symbols_root = docs_root / config.symbols_dir;

    std::unordered_set<NodeId> seen;
    for (const Node& node : report.context_nodes) {
        if (!seen.insert(node.id).second) {
            continue;
        }
        std::filesystem::path path = symbols_root / symbol_filename(node);
        std::string source = read_slice(root, node, 24000);
        if (write_if_changed(path, render_symbol_page(node, source))) {
            result.changed = true;
            result.written_files.push_back(path);
        }
    }

    std::filesystem::path index_path = docs_root / config.index_file;
    if (write_if_changed(index_path, render_index(config, report, generated_update, docs_root))) {
        result.changed = true;
        result.written_files.push_back(index_path);
    }

    std::filesystem::path manifest_path = docs_root / config.manifest_file;
    if (write_if_changed(manifest_path, render_manifest(report))) {
        result.changed = true;
        result.written_files.push_back(manifest_path);
    }

    return result;
}

}  // namespace archivum
