#pragma once

#include <archivum/graph.hpp>
#include <string>
#include <vector>

extern "C" {
typedef struct TSLanguage TSLanguage;
typedef struct TSParser TSParser;
typedef struct TSTree TSTree;
typedef struct TSNode TSNode;

const TSLanguage* tree_sitter_cpp();
}

namespace archivum {

class ASTParser {
   private:
    TSParser* parser;

    void extract_structural_nodes(TSNode root_node, std::vector<Node>& extracted_nodes, const std::string& filepath,
                                  const std::string& source_code);

    uint64_t generate_node_id(const std::string& filepath, const std::string& symbol_name) const;

   public:
    ASTParser();
    ~ASTParser();

    ASTParser(const ASTParser&) = delete;
    ASTParser& operator=(const ASTParser&) = delete;

    std::vector<Node> parse_file(const std::string& filepath);
};

}  // namespace archivum