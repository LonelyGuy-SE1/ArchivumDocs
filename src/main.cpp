#include <archivum/git_utils.hpp>
#include <archivum/graph.hpp>
#include <archivum/parser.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "[ArchivumDocs] FATAL: Missing Git coordinates.\n";
        std::cerr << "Usage: ./archivum <base_sha> <head_sha>\n";
        return 1;
    }

    std::string base_sha = argv[1];
    std::string head_sha = argv[2];

    if (base_sha == "FIRST_COMMIT") {
        std::cout << "[ArchivumDocs] Initial commit detected. Skipping diff calculation.\n";
        return 0;
    }

    std::cout << "[ArchivumDocs] Domain Expansion Initiated.\n";
    std::cout << "[ArchivumDocs] Trajectory: " << base_sha << " -> " << head_sha << "\n\n";

    archivum::init_git_subsystem();

    {
        archivum::GitScanner scanner(".");
        archivum::ASTParser parser;
        archivum::DependencyGraph graph;

        std::vector<archivum::FileDiff> diffs = scanner.calculate_diff(base_sha, head_sha);
        std::cout << "[ArchivumDocs] Scanned diff. Found " << diffs.size() << " modified files.\n";

        for (const auto& file_diff : diffs) {
            if (file_diff.file_path.ends_with(".cpp") || file_diff.file_path.ends_with(".hpp") ||
                file_diff.file_path.ends_with(".h") || file_diff.file_path.ends_with(".c")) {
                std::cout << "  -> Parsing AST for: " << file_diff.file_path << "\n";
                std::vector<archivum::Node> extracted_nodes = parser.parse_file(file_diff.file_path);

                for (const auto& node : extracted_nodes) {
                    graph.register_node(node);

                    bool mutated = false;
                    for (const auto& range : file_diff.modified_lines) {
                        if (range.start_line <= node.end_line && range.end_line >= node.start_line) {
                            mutated = true;
                            break;
                        }
                    }

                    if (mutated) {
                        std::string type_str = (node.type == archivum::NodeType::FUNCTION) ? "FUNCTION"
                                               : (node.type == archivum::NodeType::CLASS)  ? "CLASS"
                                                                                           : "STRUCT";

                        std::cout << "     [MUTATED " << type_str << "] " << node.name << " (Lines " << node.start_line
                                  << "-" << node.end_line << ")\n";
                    }
                }
            }
        }
    }

    archivum::shutdown_git_subsystem();

    std::cout << "\n[ArchivumDocs] Intersection complete. Standing by for LLM dispatch.\n";
    return 0;
}