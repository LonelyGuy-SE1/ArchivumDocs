#include <vector>
#include <archivum/parser.hpp>
#include <fstream>
#include <sstream>
#include <functional>

extern "C"{
    #include <tree_sitter/api.h>
}

namespace archivum{
    //memory alloc
    ASTParser::ASTParser(){
        parser=ts_parser_new();
        ts_parser_set_language(parser, tree_sitter_cpp());
    }
    //RAII enforcer
    ASTParser::~ASTParser(){
        if(parser!=nullptr){
        ts_parser_delete(parser);
        }
    }

    //64 bit id generator
    uint64_t ASTParser::generate_node_id(const std::string& filename, const std::string& symbol_name) const{
       std::string composite_key=filename+"::"+symbol_name;
       return std::hash<std::string>{}(composite_key); 
    }

    //file io + tree sitter exec
    std::vector<Node> ASTParser::parse_file(const std::string& filepath){
        std::vector<Node> extracted_nodes;

        std::ifstream file(filepath);
        if(!file.is_open()){
            std::cerr<<"[ArchivumDocs] ERROR: Failed to open file "<<filepath<<std::endl;
            return extracted_nodes;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source_code = buffer.str();

        TSTree* tree=ts_parser_parse_string(
            parser,
            nullptr,
            source_code.c_str(),
            source_code.length()
        );
    if(!tree){
        std::cerr<<"[ArchivumDocs] FATAL: Tree-sitter failed to parse file "<<filepath<<std::endl;
        return extracted_nodes;
    }
    TSNode root_node=ts_tree_root_node(tree);
    extract_structural_nodes(root_node, extracted_nodes, filepath);
    ts_tree_delete(tree);
    return extracted_nodes;
    }
void ASTParser::extract_structural_nodes(TSNode ts_node, std::vector<Node>& extracted_nodes, const std::string& filepath){
//tbw
}
}
