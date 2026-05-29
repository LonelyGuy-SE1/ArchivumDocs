# DependencyGraph::get_node

- Type: function
- Source: `src/graph.cpp:92-98`
- Interface hash: `3897427822453049738`
- Source hash: `15696320602603809128`

## Signature

```cpp
const Node& DependencyGraph::get_node(NodeId id) const
```

## References

- `DependencyGraph`
- `Node`
- `NodeId`
- `end`
- `find`
- `id`
- `it`
- `registry`
- `runtime_error`
- `second`
- `std`

## Source

```cpp
const Node& DependencyGraph::get_node(NodeId id) const {
    auto it = registry.find(id);
    if (it != registry.end()) {
        return it->second;
    }
    throw std::runtime_error("[ArchivumDocs] FATAL: Attempted to access unregistered Node ID.");
}

```
