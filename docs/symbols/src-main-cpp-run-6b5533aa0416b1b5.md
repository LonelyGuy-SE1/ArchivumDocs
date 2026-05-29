# run

- Type: function
- Source: `src/main.cpp:241-334`
- Interface hash: `7998046516298296678`
- Source hash: `13857618139881685442`

## Signature

```cpp
int run(int argc, char* argv[])
```

## References

- `ASTParser`
- `AnalysisReport`
- `ArchivumConfig`
- `CliOptions`
- `DependencyGraph`
- `DispatchSummary`
- `DocumentationWriteResult`
- `ExecutionRange`
- `FileDiff`
- `GitRuntime`
- `GitScanner`
- `ModifiedRange`
- `Node`
- `NodeId`
- `archivum`
- `argc`
- `argv`
- `base_sha`
- `begin`
- `build_documentation_prompt`
- `calculate_blast_radius`
- `calculate_diff`
- `cap_vector`
- `changed`
- `changed_file_count`
- `changed_ranges`
- `config`
- `config_path`
- `context_nodes`
- `context_symbols`
- `coordinates`
- `cout`
- `credentials_available`
- `diff`
- `diffs`
- `discover_source_files`
- `dispatch`
- `docs`
- `docs_dir`
- `edge_count`
- `end`
- `file`
- `file_path`
- `find`
- `full_repository_diffs`
- `generate_documentation_update`
- `generated`
- `generic_string`
- `graph`
- `graph_edge_count`
- `graph_node_count`
- `head_sha`
- `id`
- `impacted`
- `impacted_nodes`
- `infer_dependencies`
- `initial_scan`
- `insert`
- `it`
- `load_config`
- `max_symbols`
- `modified_lines`
- `mutated`
- `mutated_nodes`
- `node`
- `node_count`
- `nodes`
- `optional`
- `options`
- `overlaps`
- `parse_cli`
- `parse_file`
- `parser`
- `print_nodes`
- `prompt`
- `provider`
- `provider_request`
- `radius`
- `range`
- `range_source`
- `register_node`
- `report`
- `reserve`
- `resolve_range`
- `runtime`
- `scanner`
- `second`
- `size`
- `sorted_nodes`
- `source`
- `source_file_count`
- `source_files`
- `std`
- `string`
- `summarize_dispatch`
- `unordered_map`
- `unordered_set`
- `update_docs`
- `value_or`
- `vector`
- `write_documentation`
- `written_files`

## Source

```cpp
int run(int argc, char* argv[]) {
    CliOptions options = parse_cli(argc, argv);
    archivum::ArchivumConfig config = archivum::load_config(options.config_path);
    GitRuntime runtime;
    archivum::GitScanner scanner(".");
    ExecutionRange range = resolve_range(options.coordinates, scanner);

    std::cout << "[ArchivumDocs] Domain expansion initiated.\n";
    std::cout << "[ArchivumDocs] Range: " << range.base_sha << " -> " << range.head_sha << " (" << range.source
              << ")\n";
    std::cout << "[ArchivumDocs] Config: " << options.config_path.generic_string() << "\n";

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
    cap_vector(impacted_nodes, config.max_symbols);

    print_nodes("Mutated symbols", mutated_nodes, 32);
    print_nodes("Context symbols", impacted_nodes, 32);

    archivum::DispatchSummary dispatch = archivum::summarize_dispatch(graph, mutated, impacted);
    std::cout << "[ArchivumDocs] Dispatch: provider=" << dispatch.provider
              << ", credentials=" << (dispatch.credentials_available ? "present" : "absent")
              << ", context_symbols=" << dispatch.context_symbols << "\n";

    if (config.update_docs) {
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

        std::string prompt = archivum::build_documentation_prompt(config, report, ".");
        std::optional<std::string> generated =
            archivum::generate_documentation_update(provider_request(config, prompt));
        archivum::DocumentationWriteResult docs =
            archivum::write_documentation(config, report, generated.value_or(""), ".");

        std::cout << "[ArchivumDocs] Documentation: " << (docs.changed ? "updated" : "unchanged") << " in "
                  << config.docs_dir << "\n";
        std::cout << "[ArchivumDocs] Documentation files touched: " << docs.written_files.size() << "\n";
    }

    std::cout << "[ArchivumDocs] Intersection complete.\n";

    return 0;
}

```
