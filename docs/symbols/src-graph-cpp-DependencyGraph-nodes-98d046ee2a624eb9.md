# DependencyGraph::nodes

- Type: function
- Source: `src/graph.cpp:100-116`
- Interface hash: `18393230026508382801`
- Source hash: `15578566504292320421`

## Signature

```cpp
std::vector<Node> DependencyGraph::nodes() const
```

## References

- `DependencyGraph`
- `Node`
- `begin`
- `end`
- `file_path`
- `id`
- `left`
- `name`
- `node`
- `push_back`
- `registry`
- `reserve`
- `right`
- `size`
- `sort`
- `start_line`
- `std`
- `values`
- `vector`

## Source

```cpp
std::vector<Node> DependencyGraph::nodes() const {
    std::vector<Node> values;
    values.reserve(registry.size());
    for (const auto& [id, node] : registry) {
        values.push_back(node);
    }
    std::sort(values.begin(), values.end(), [](const Node& left, const Node& right) {
        if (left.file_path != right.file_path) {
            return left.file_path < right.file_path;
        }
        if (left.start_line != right.start_line) {
            return left.start_line < right.start_line;
        }
        return left.name < right.name;
    });
    return values;
}

```
