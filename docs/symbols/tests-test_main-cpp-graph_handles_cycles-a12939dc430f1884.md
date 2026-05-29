# graph_handles_cycles

- Type: function
- Source: `tests/test_main.cpp:133-144`
- Interface hash: `15693871238517536844`
- Source hash: `9910672154146366647`

## Signature

```cpp
void graph_handles_cycles()
```

## References

- `DependencyGraph`
- `NodeId`
- `add_dependency`
- `archivum`
- `calculate_blast_radius`
- `graph`
- `make_node`
- `radius`
- `register_node`
- `require`
- `size`
- `std`
- `unordered_set`

## Source

```cpp
void graph_handles_cycles() {
    archivum::DependencyGraph graph;
    graph.register_node(make_node(1, "A", {}));
    graph.register_node(make_node(2, "B", {}));
    graph.register_node(make_node(3, "C", {}));
    graph.add_dependency(1, 2);
    graph.add_dependency(2, 3);
    graph.add_dependency(3, 1);

    std::unordered_set<archivum::NodeId> radius = graph.calculate_blast_radius(1);
    require(radius.size() == 3, "cycle traversal did not converge");
}

```
