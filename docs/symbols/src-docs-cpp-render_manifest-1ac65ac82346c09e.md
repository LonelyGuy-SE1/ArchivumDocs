# render_manifest

- Type: function
- Source: `src/docs.cpp:182-195`
- Interface hash: `13304531271069190795`
- Source hash: `9116513786653973816`

## Signature

```cpp
std::string render_manifest(const AnalysisReport& report)
```

## References

- `AnalysisReport`
- `base_sha`
- `changed_file_count`
- `context_nodes`
- `graph_edge_count`
- `graph_node_count`
- `head_sha`
- `mutated_nodes`
- `ostringstream`
- `out`
- `report`
- `size`
- `source_file_count`
- `std`
- `str`
- `string`

## Source

```cpp
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

```
