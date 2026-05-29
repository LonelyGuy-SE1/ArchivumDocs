# sorted_nodes

- Type: function
- Source: `src/main.cpp:183-204`
- Interface hash: `3751818328144918680`
- Source hash: `5426219290315427687`

## Signature

```cpp
std::vector<archivum::Node> sorted_nodes(const std::unordered_set<archivum::NodeId>& ids, const archivum::DependencyGraph& graph)
```

## References

- `DependencyGraph`
- `Node`
- `NodeId`
- `archivum`
- `begin`
- `contains`
- `end`
- `file_path`
- `get_node`
- `graph`
- `id`
- `ids`
- `left`
- `name`
- `nodes`
- `push_back`
- `reserve`
- `right`
- `size`
- `sort`
- `start_line`
- `std`
- `unordered_set`
- `vector`

## Source

```cpp
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

```
