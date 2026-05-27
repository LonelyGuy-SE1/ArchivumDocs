#include <algorithm>
#include <archivum/api.hpp>
#include <archivum/git_utils.hpp>
#include <archivum/graph.hpp>
#include <archivum/parser.hpp>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

constexpr const char* kInitialCommit = "FIRST_COMMIT";

struct ExecutionRange {
    std::string base_sha;
    std::string head_sha;
    std::string source;
    bool initial_scan;
};

class GitRuntime {
   public:
    GitRuntime() { archivum::init_git_subsystem(); }
    ~GitRuntime() { archivum::shutdown_git_subsystem(); }

    GitRuntime(const GitRuntime&) = delete;
    GitRuntime& operator=(const GitRuntime&) = delete;
};

bool is_source_file(const std::filesystem::path& path) {
    std::string extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    return extension == ".c" || extension == ".cc" || extension == ".cpp" || extension == ".cxx" || extension == ".h" ||
           extension == ".hh" || extension == ".hpp" || extension == ".hxx" || extension == ".ipp";
}

bool ignored_directory(const std::filesystem::path& path) {
    std::string name = path.filename().string();
    return name == ".git" || name == ".github" || name == ".vscode" || name == "build" || name == "docs";
}

std::vector<std::string> discover_source_files(const std::filesystem::path& root) {
    std::vector<std::string> files;
    std::filesystem::recursive_directory_iterator it(root, std::filesystem::directory_options::skip_permission_denied);
    std::filesystem::recursive_directory_iterator end;

    for (; it != end; ++it) {
        const std::filesystem::directory_entry& entry = *it;
        if (entry.is_directory()) {
            if (ignored_directory(entry.path())) {
                it.disable_recursion_pending();
            }
            continue;
        }

        if (!entry.is_regular_file() || !is_source_file(entry.path())) {
            continue;
        }

        files.push_back(std::filesystem::relative(entry.path(), root).generic_string());
    }

    std::sort(files.begin(), files.end());
    return files;
}

std::vector<archivum::FileDiff> full_repository_diffs(const std::vector<std::string>& files) {
    std::vector<archivum::FileDiff> diffs;
    diffs.reserve(files.size());

    for (const std::string& file : files) {
        archivum::FileDiff diff;
        diff.file_path = file;
        diff.modified_lines.push_back({1, std::numeric_limits<uint32_t>::max()});
        diffs.push_back(std::move(diff));
    }

    return diffs;
}

ExecutionRange resolve_range(int argc, char* argv[], archivum::GitScanner& scanner) {
    if (argc == 1) {
        std::string head = scanner.resolve_reference("HEAD");
        std::optional<std::string> parent = scanner.parent_of(head);
        return {parent.value_or(kInitialCommit), head, parent.has_value() ? "auto" : "initial", !parent.has_value()};
    }

    if (argc == 2) {
        std::string arg = argv[1];
        std::string head = scanner.resolve_reference(arg);
        std::optional<std::string> parent = scanner.parent_of(head);
        return {parent.value_or(kInitialCommit), head, "head-ref", !parent.has_value()};
    }

    if (argc == 3) {
        std::string base = argv[1];
        std::string head = scanner.resolve_reference(argv[2]);
        bool initial = base == kInitialCommit;
        if (!initial) {
            base = scanner.resolve_reference(base);
        }
        return {base, head, "explicit", initial};
    }

    throw std::runtime_error("[ArchivumDocs] FATAL: Expected zero, one, or two Git coordinates.");
}

bool overlaps(const archivum::Node& node, const std::vector<archivum::ModifiedRange>& ranges) {
    for (const archivum::ModifiedRange& range : ranges) {
        if (range.start_line <= node.end_line && range.end_line >= node.start_line) {
            return true;
        }
    }
    return false;
}

std::string type_name(archivum::NodeType type) {
    switch (type) {
        case archivum::NodeType::FUNCTION:
            return "function";
        case archivum::NodeType::CLASS:
            return "class";
        case archivum::NodeType::STRUCT:
            return "struct";
        case archivum::NodeType::METHOD:
            return "method";
        default:
            return "symbol";
    }
}

std::vector<archivum::Node> sorted_nodes(const std::unordered_set<archivum::NodeId>& ids,
                                         const archivum::DependencyGraph& graph) {
    std::vector<archivum::Node> nodes;
    nodes.reserve(ids.size());
    for (archivum::NodeId id : ids) {
        if (graph.contains(id)) {
            nodes.push_back(graph.get_node(id));
        }
    }

    std::sort(nodes.begin(), nodes.end(), [](const archivum::Node& left, const archivum::Node& right) {
        if (left.file_path != right.file_path) {
            return left.file_path < right.file_path;
        }
        if (left.start_line != right.start_line) {
            return left.start_line < right.start_line;
        }
        return left.name < right.name;
    });

    return nodes;
}

void print_nodes(const std::string& title, const std::vector<archivum::Node>& nodes, size_t limit) {
    std::cout << "[ArchivumDocs] " << title << ": " << nodes.size() << "\n";

    size_t shown = std::min(limit, nodes.size());
    for (size_t index = 0; index < shown; ++index) {
        const archivum::Node& node = nodes[index];
        std::cout << "  - " << type_name(node.type) << " " << node.name << " [" << node.file_path << ":"
                  << node.start_line << "-" << node.end_line << "]\n";
    }

    if (nodes.size() > shown) {
        std::cout << "  - +" << (nodes.size() - shown) << " more\n";
    }
}

int run(int argc, char* argv[]) {
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: archivum [base_sha head_sha]\n";
            std::cout << "       archivum [head_ref]\n";
            std::cout << "       archivum\n";
            return 0;
        }
        if (arg == "--version") {
            std::cout << "ArchivumDocs 1.0.0\n";
            return 0;
        }
    }

    GitRuntime runtime;
    archivum::GitScanner scanner(".");
    ExecutionRange range = resolve_range(argc, argv, scanner);

    std::cout << "[ArchivumDocs] Domain expansion initiated.\n";
    std::cout << "[ArchivumDocs] Range: " << range.base_sha << " -> " << range.head_sha << " (" << range.source
              << ")\n";

    std::vector<std::string> source_files = discover_source_files(".");
    std::vector<archivum::FileDiff> diffs = range.initial_scan ? full_repository_diffs(source_files)
                                                               : scanner.calculate_diff(range.base_sha, range.head_sha);

    archivum::ASTParser parser;
    archivum::DependencyGraph graph;

    for (const std::string& file : source_files) {
        for (const archivum::Node& node : parser.parse_file(file)) {
            graph.register_node(node);
        }
    }

    graph.infer_dependencies();

    std::unordered_map<std::string, std::vector<archivum::ModifiedRange>> changed_ranges;
    changed_ranges.reserve(diffs.size());
    for (const archivum::FileDiff& diff : diffs) {
        changed_ranges[diff.file_path].insert(changed_ranges[diff.file_path].end(), diff.modified_lines.begin(),
                                              diff.modified_lines.end());
    }

    std::unordered_set<archivum::NodeId> mutated;
    for (const archivum::Node& node : graph.nodes()) {
        auto it = changed_ranges.find(node.file_path);
        if (it != changed_ranges.end() && overlaps(node, it->second)) {
            mutated.insert(node.id);
        }
    }

    std::unordered_set<archivum::NodeId> impacted;
    for (archivum::NodeId id : mutated) {
        std::unordered_set<archivum::NodeId> radius = graph.calculate_blast_radius(id);
        impacted.insert(radius.begin(), radius.end());
    }

    std::cout << "[ArchivumDocs] Source files indexed: " << source_files.size() << "\n";
    std::cout << "[ArchivumDocs] Changed files scanned: " << diffs.size() << "\n";
    std::cout << "[ArchivumDocs] Repository graph: " << graph.node_count() << " symbols, " << graph.edge_count()
              << " inferred edges\n";

    std::vector<archivum::Node> mutated_nodes = sorted_nodes(mutated, graph);
    std::vector<archivum::Node> impacted_nodes = sorted_nodes(impacted, graph);

    print_nodes("Mutated symbols", mutated_nodes, 32);
    print_nodes("Context symbols", impacted_nodes, 32);

    archivum::DispatchSummary dispatch = archivum::summarize_dispatch(graph, mutated, impacted);
    std::cout << "[ArchivumDocs] Dispatch: provider=" << dispatch.provider
              << ", credentials=" << (dispatch.credentials_available ? "present" : "absent")
              << ", context_symbols=" << dispatch.context_symbols << "\n";
    std::cout << "[ArchivumDocs] Intersection complete.\n";

    return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        return run(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << error.what() << "\n";
        return 1;
    }
}
