#include <archivum/graph.hpp>
#include <archivum/parser.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "[ArchivumDocs] Core Engine Online.\n" << std::endl;

    archivum::ASTParser parser;
    archivum::DependencyGraph graph;

    std::string target_file = "src/parser.cpp";
    std::cout << "[ArchivumDocs] Executing AST extraction on: " << target_file << " ...\n";

    std::vector<archivum::Node> extracted_nodes = parser.parse_file(target_file);

    if (extracted_nodes.empty()) {
        std::cerr << "[ArchivumDocs] WARNING: No structural nodes extracted. Check "
                     "file path."
                  << std::endl;
        return 1;
    }

    std::cout << "[ArchivumDocs] Extraction complete. Found " << extracted_nodes.size()
              << " structural boundaries.\n\n";

    for (const auto& node : extracted_nodes) {
        graph.register_node(node);

        std::string type_str = (node.type == archivum::NodeType::FUNCTION) ? "FUNCTION"
                               : (node.type == archivum::NodeType::CLASS)  ? "CLASS"
                                                                           : "STRUCT";

        std::cout << "  -> [" << type_str << "] " << node.name << " \t(Lines " << node.start_line << " to "
                  << node.end_line << ")"
                  << " \tID: " << node.id << "\n";
    }

    std::cout << "\n[ArchivumDocs] Graph Registry populated. Awaiting diff "
                 "calculations.\n";
    return 0;
}