# DependencyGraph::contains

- Type: function
- Source: `src/graph.cpp:90-90`
- Interface hash: `4555315540614517243`
- Source hash: `18343194846255398085`

## Signature

```cpp
bool DependencyGraph::contains(NodeId id) const
```

## References

- `DependencyGraph`
- `NodeId`
- `end`
- `find`
- `id`
- `registry`

## Source

```cpp
bool DependencyGraph::contains(NodeId id) const { return registry.find(id) != registry.end(); }

```
