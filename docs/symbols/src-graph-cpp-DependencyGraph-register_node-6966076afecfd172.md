# DependencyGraph::register_node

- Type: function
- Source: `src/graph.cpp:22-22`
- Interface hash: `302948202092422968`
- Source hash: `12829867606171676778`

## Signature

```cpp
void DependencyGraph::register_node(const Node& node)
```

## References

- `DependencyGraph`
- `Node`
- `id`
- `node`
- `registry`

## Source

```cpp
void DependencyGraph::register_node(const Node& node) { registry[node.id] = node; }

```
