# AnalysisReport

- Type: struct
- Source: `include/archivum/docs.hpp:13-25`
- Interface hash: `16891267611902205352`
- Source hash: `12116822413444291699`

## Signature

```cpp
struct AnalysisReport
```

## References

- `FileDiff`
- `Node`
- `base_sha`
- `changed_file_count`
- `context_nodes`
- `diffs`
- `graph_edge_count`
- `graph_node_count`
- `head_sha`
- `initial_scan`
- `mutated_nodes`
- `range_source`
- `source_file_count`
- `std`
- `string`
- `vector`

## Source

```cpp
struct AnalysisReport {
    std::string base_sha;
    std::string head_sha;
    std::string range_source;
    bool initial_scan = false;
    size_t source_file_count = 0;
    size_t changed_file_count = 0;
    size_t graph_node_count = 0;
    size_t graph_edge_count = 0;
    std::vector<FileDiff> diffs;
    std::vector<Node> mutated_nodes;
    std::vector<Node> context_nodes;
};

```
