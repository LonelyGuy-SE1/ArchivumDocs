#include <algorithm>
#include <archivum/api.hpp>
#include <archivum/config.hpp>
#include <archivum/docs.hpp>
#include <archivum/git_utils.hpp>
#include <archivum/graph.hpp>
#include <archivum/parser.hpp>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
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

struct CliOptions {
    std::filesystem::path config_path = ".archivum.yml";
    std::vector<std::string> coordinates;
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

bool ignored_directory(const std::filesystem::path& path, const std::unordered_set<std::string>& ignore_dirs) {
    std::string name = path.filename().string();
    return ignore_dirs.find(name) != ignore_dirs.end();
}

std::vector<std::string> discover_source_files(const std::filesystem::path& root,
                                               const std::unordered_set<std::string>& ignore_dirs) {
    std::vector<std::string> files;
    std::filesystem::recursive_directory_iterator it(root, std::filesystem::directory_options::skip_permission_denied);
    std::filesystem::recursive_directory_iterator end;

    for (; it != end; ++it) {
        const std::filesystem::directory_entry& entry = *it;
        if (entry.is_directory()) {
            if (ignored_directory(entry.path(), ignore_dirs)) {
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

CliOptions parse_cli(int argc, char* argv[]) {
    CliOptions options;

    for (int index = 1; index < argc; ++index) {
        std::string arg = argv[index];
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: archivum [--config path] [base_sha head_sha]\n";
            std::cout << "       archivum [--config path] [head_ref]\n";
            std::cout << "       archivum [--config path]\n";
            std::exit(0);
        }
        if (arg == "--version") {
            std::cout << "ArchivumDocs 1.0.0\n";
            std::exit(0);
        }
        if (arg == "--config") {
            if (index + 1 >= argc) {
                throw std::runtime_error("[ArchivumDocs] FATAL: --config requires a path.");
            }
            options.config_path = argv[++index];
            continue;
        }
        options.coordinates.push_back(arg);
    }

    if (options.coordinates.size() > 2) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Expected zero, one, or two Git coordinates.");
    }

    return options;
}

ExecutionRange resolve_range(const std::vector<std::string>& coordinates, archivum::GitScanner& scanner) {
    if (coordinates.empty()) {
        std::string head = scanner.resolve_reference("HEAD");
        std::optional<std::string> parent = scanner.parent_of(head);
        return {parent.value_or(kInitialCommit), head, parent.has_value() ? "auto" : "initial", !parent.has_value()};
    }

    if (coordinates.size() == 1) {
        std::string arg = coordinates[0];
        std::string head = scanner.resolve_reference(arg);
        std::optional<std::string> parent = scanner.parent_of(head);
        return {parent.value_or(kInitialCommit), head, "head-ref", !parent.has_value()};
    }

    if (coordinates.size() == 2) {
        std::string base = coordinates[0];
        std::string head = scanner.resolve_reference(coordinates[1]);
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

template <typename T>
void cap_vector(std::vector<T>& values, size_t limit) {
    if (values.size() > limit) {
        values.resize(limit);
    }
}

archivum::ProviderRequest provider_request(const archivum::ArchivumConfig& config, const std::string& prompt) {
    archivum::ProviderRequest request;
    request.provider = config.provider == "auto" ? "openai" : config.provider;
    request.endpoint = config.endpoint;
    request.model = config.model;
    request.api_key_env = config.api_key_env;
    request.reasoning_effort = config.reasoning_effort;
    request.verbosity = config.verbosity;
    request.prompt = prompt;
    request.fail_on_error = config.fail_on_provider_error;
    return request;
}

int run(int argc, char* argv[]) {
    CliOptions options = parse_cli(argc, argv);
    archivum::ArchivumConfig config = archivum::load_config(options.config_path);
    std::filesystem::path docs_root = std::filesystem::path(".") / config.docs_dir;
    std::filesystem::path index_path = docs_root / config.index_file;
    std::filesystem::path manifest_path = docs_root / config.manifest_file;
    bool docs_ready = std::filesystem::exists(index_path) && std::filesystem::exists(manifest_path);
    GitRuntime runtime;
    archivum::GitScanner scanner(".");
    ExecutionRange range = resolve_range(options.coordinates, scanner);

    std::unordered_set<std::string> ignore_dirs(config.ignore_dirs.begin(), config.ignore_dirs.end());

    if (config.update_docs && !docs_ready) {
        range.initial_scan = true;
        range.base_sha = kInitialCommit;
        range.source = "docs-missing";
    }

    std::cout << "[ArchivumDocs] Domain expansion initiated.\n";
    std::cout << "[ArchivumDocs] Range: " << range.base_sha << " -> " << range.head_sha << " (" << range.source
              << ")\n";
    std::cout << "[ArchivumDocs] Config: " << options.config_path.generic_string() << "\n";

    std::vector<std::string> source_files = discover_source_files(".", ignore_dirs);
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
    cap_vector(impacted_nodes, config.max_symbols);

    print_nodes("Mutated symbols", mutated_nodes, 32);
    print_nodes("Context symbols", impacted_nodes, 32);

    archivum::DispatchSummary dispatch = archivum::summarize_dispatch(graph, mutated, impacted);
    std::cout << "[ArchivumDocs] Dispatch: provider=" << dispatch.provider
              << ", credentials=" << (dispatch.credentials_available ? "present" : "absent")
              << ", context_symbols=" << dispatch.context_symbols;
    if (dispatch.provider == "auto" && !dispatch.credentials_available) {
        std::cout << " (auto-select fell back to local output)";
    }
    std::cout << "\n";

    bool should_update_docs = config.update_docs &&
                              (range.initial_scan || !docs_ready || !mutated_nodes.empty() || !impacted_nodes.empty());

    if (should_update_docs) {
        archivum::AnalysisReport report;
        report.base_sha = range.base_sha;
        report.head_sha = range.head_sha;
        report.range_source = range.source;
        report.initial_scan = range.initial_scan;
        report.source_file_count = source_files.size();
        report.changed_file_count = diffs.size();
        report.graph_node_count = graph.node_count();
        report.graph_edge_count = graph.edge_count();
        report.diffs = diffs;
        report.mutated_nodes = mutated_nodes;
        report.context_nodes = impacted_nodes;

        std::string existing_docs;
        if (std::filesystem::exists(index_path)) {
            std::ifstream index_file(index_path);
            if (index_file.is_open()) {
                std::stringstream buffer;
                buffer << index_file.rdbuf();
                existing_docs = buffer.str();
            }
        }

        std::string prompt = archivum::build_documentation_prompt(config, report, ".", existing_docs);
        std::optional<std::string> generated =
            archivum::generate_documentation_update(provider_request(config, prompt));
        archivum::DocumentationWriteResult docs =
            archivum::write_documentation(config, report, generated.value_or(""), ".");
        archivum::ContextMap context_map = archivum::build_context_map(graph, mutated_nodes, impacted_nodes);
        archivum::write_context_map(config, context_map, ".", docs);

        std::cout << "[ArchivumDocs] Documentation: " << (docs.changed ? "updated" : "unchanged") << " in "
                  << config.docs_dir << "\n";
        std::cout << "[ArchivumDocs] Documentation files touched: " << docs.written_files.size() << "\n";
    } else if (!config.update_docs) {
        std::cout << "[ArchivumDocs] Documentation updates disabled.\n";
    } else {
        std::cout << "[ArchivumDocs] Documentation up to date; no changes required.\n";
    }

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
