# render_index

- Type: function
- Source: `src/docs.cpp:140-180`
- Interface hash: `17290089167313208972`
- Source hash: `13523203640581418919`

## Signature

```cpp
std::string render_index(const ArchivumConfig& config, const AnalysisReport& report, const std::string& generated_update, const std::filesystem::path& docs_root)
```

## References

- `AnalysisReport`
- `ArchivumConfig`
- `Node`
- `base_sha`
- `changed_file_count`
- `config`
- `context_nodes`
- `docs_root`
- `empty`
- `end_line`
- `file_path`
- `filesystem`
- `generated_update`
- `graph_edge_count`
- `graph_node_count`
- `head_sha`
- `index_file`
- `mutated_nodes`
- `name`
- `node`
- `ostringstream`
- `out`
- `path`
- `relative_link`
- `report`
- `short_sha`
- `size`
- `source_file_count`
- `start_line`
- `std`
- `str`
- `string`
- `symbol_filename`
- `symbol_path`
- `symbols_dir`
- `type`
- `type_name`
- `write_mode`
- `write_mode_name`

## Source

```cpp
std::string render_index(const ArchivumConfig& config, const AnalysisReport& report,
                         const std::string& generated_update, const std::filesystem::path& docs_root) {
    std::ostringstream out;
    out << "# ArchivumDocs\n\n";
    out << "## Current Update\n\n";
    out << "- Range: `" << short_sha(report.base_sha) << "` -> `" << short_sha(report.head_sha) << "`\n";
    out << "- Source files indexed: " << report.source_file_count << "\n";
    out << "- Changed files scanned: " << report.changed_file_count << "\n";
    out << "- Repository graph: " << report.graph_node_count << " symbols, " << report.graph_edge_count << " edges\n";
    out << "- Mutated symbols: " << report.mutated_nodes.size() << "\n";
    out << "- Context symbols: " << report.context_nodes.size() << "\n";
    out << "- Write mode: `" << write_mode_name(config.write_mode) << "`\n";

    if (!generated_update.empty()) {
        out << "\n## AI Update\n\n" << generated_update << "\n";
    }

    out << "\n## Mutated Symbols\n\n";
    if (report.mutated_nodes.empty()) {
        out << "No structural code changes were detected for this range.\n";
    } else {
        for (const Node& node : report.mutated_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "- [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") ";
            out << "`" << node.file_path << ":" << node.start_line << "-" << node.end_line << "`\n";
        }
    }

    out << "\n## Context Symbols\n\n";
    if (report.context_nodes.empty()) {
        out << "No downstream documentation context was required.\n";
    } else {
        for (const Node& node : report.context_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "- [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") ";
            out << "`" << type_name(node.type) << "`\n";
        }
    }

    return out.str();
}

```
