# DependencyGraph::node_count

- Type: function
- Source: `src/graph.cpp:129-129`
- Interface hash: `17773914781795580924`
- Source hash: `5199096871569576778`

## Signature

```cpp
size_t DependencyGraph::node_count() const
```

## References

- `DependencyGraph`
- `registry`
- `size`

## Source

```cpp
size_t DependencyGraph::node_count() const { return registry.size(); }

```
