# DependencyGraph::add_dependency

- Type: function
- Source: `src/graph.cpp:24-29`
- Interface hash: `11673616971255384769`
- Source hash: `12256255536103709234`

## Signature

```cpp
void DependencyGraph::add_dependency(NodeId provider, NodeId consumer)
```

## References

- `DependencyGraph`
- `NodeId`
- `consumer`
- `downstream_edges`
- `end`
- `find`
- `insert`
- `provider`
- `registry`

## Source

```cpp
void DependencyGraph::add_dependency(NodeId provider, NodeId consumer) {
    if (provider != consumer && registry.find(provider) != registry.end() &&
        registry.find(consumer) != registry.end()) {
        downstream_edges[provider].insert(consumer);
    }
}

```
