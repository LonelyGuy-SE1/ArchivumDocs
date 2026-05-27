#include <archivum/parser.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

extern "C" {
#include <tree_sitter/api.h>
}

namespace archivum {

ASTParser::ASTParser() {
  parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_cpp());
}

ASTParser::~ASTParser() {
  if (parser != nullptr) {
    ts_parser_delete(parser);
  }
}

uint64_t ASTParser::generate_node_id(const std::string &filepath,
                                     const std::string &symbol_name) const {
  std::string composite_key = filepath + "::" + symbol_name;
  return std::hash<std::string>{}(composite_key);
}

std::vector<Node> ASTParser::parse_file(const std::string &filepath) {
  std::vector<Node> extracted_nodes;

  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "[ArchivumDocs] ERROR: Could not open file: " << filepath
              << std::endl;
    return extracted_nodes;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string source_code = buffer.str();

  TSTree *tree = ts_parser_parse_string(parser, nullptr, source_code.c_str(),
                                        source_code.length());

  if (!tree) {
    std::cerr << "[ArchivumDocs] FATAL: Tree-sitter failed to parse file."
              << std::endl;
    return extracted_nodes;
  }

  TSNode root_node = ts_tree_root_node(tree);

  extract_structural_nodes(root_node, extracted_nodes, filepath, source_code);

  ts_tree_delete(tree);

  return extracted_nodes;
}

void ASTParser::extract_structural_nodes(TSNode root_node,
                                         std::vector<Node> &extracted_nodes,
                                         const std::string &filepath,
                                         const std::string &source_code) {
  TSTreeCursor cursor = ts_tree_cursor_new(root_node);
  bool reached_root = false;

  while (!reached_root) {
    TSNode current_node = ts_tree_cursor_current_node(&cursor);
    std::string type = ts_node_type(current_node);

    if (type == "function_definition" || type == "class_specifier" ||
        type == "struct_specifier") {
      Node current;
      current.file_path = filepath;

      TSPoint start_point = ts_node_start_point(current_node);
      TSPoint end_point = ts_node_end_point(current_node);
      current.start_line = start_point.row + 1;
      current.end_line = end_point.row + 1;

      if (type == "function_definition")
        current.type = NodeType::FUNCTION;
      else if (type == "class_specifier")
        current.type = NodeType::CLASS;
      else
        current.type = NodeType::STRUCT;

      TSNode name_node =
          ts_node_child_by_field_name(current_node, "declarator", 10);
      if (ts_node_is_null(name_node)) {
        name_node = ts_node_child_by_field_name(current_node, "name", 4);
      }

      if (!ts_node_is_null(name_node)) {
        uint32_t start_byte = ts_node_start_byte(name_node);
        uint32_t end_byte = ts_node_end_byte(name_node);
        current.name = source_code.substr(start_byte, end_byte - start_byte);
      } else {
        current.name =
            "unnamed_" + type + "_" + std::to_string(current.start_line);
      }

      current.id = generate_node_id(filepath, current.name);
      current.interface_hash = 0;

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
} // namespace archivum