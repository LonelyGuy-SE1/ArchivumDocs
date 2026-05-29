# DependencyGraph::downstream

- Type: function
- Source: `src/graph.cpp:118-127`
- Interface hash: `5923069891756264639`
- Source hash: `9146541921012893205`

## Signature

```cpp
std::vector<NodeId> DependencyGraph::downstream(NodeId id) const
```

## References

- `DependencyGraph`
- `NodeId`
- `assign`
- `begin`
- `downstream_edges`
- `end`
- `find`
- `id`
- `it`
- `second`
- `sort`
- `std`
- `values`
- `vector`

## Source

```cpp
std::vector<NodeId> DependencyGraph::downstream(NodeId id) const {
    std::vector<NodeId> values;
    auto it = downstream_edges.find(id);
    if (it == downstream_edges.end()) {
        return values;
    }
    values.assign(it->second.begin(), it->second.end());
    std::sort(values.begin(), values.end());
    return values;
}

```
