# DependencyGraph::edge_count

- Type: function
- Source: `src/graph.cpp:131-137`
- Interface hash: `13390147643126773137`
- Source hash: `11322795324188378293`

## Signature

```cpp
size_t DependencyGraph::edge_count() const
```

## References

- `DependencyGraph`
- `downstream_edges`
- `edges`
- `id`
- `size`
- `total`

## Source

```cpp
size_t DependencyGraph::edge_count() const {
    size_t total = 0;
    for (const auto& [id, edges] : downstream_edges) {
        total += edges.size();
    }
    return total;
}

```
