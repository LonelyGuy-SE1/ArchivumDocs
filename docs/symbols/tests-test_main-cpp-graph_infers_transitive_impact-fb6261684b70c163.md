# graph_infers_transitive_impact

- Type: function
- Source: `tests/test_main.cpp:121-131`
- Interface hash: `5558540940780532055`
- Source hash: `15211726910546162015`

## Signature

```cpp
void graph_infers_transitive_impact()
```

## References

- `DependencyGraph`
- `NodeId`
- `archivum`
- `calculate_blast_radius`
- `edge_count`
- `graph`
- `infer_dependencies`
- `make_node`
- `radius`
- `register_node`
- `require`
- `size`
- `std`
- `unordered_set`

## Source

```cpp
void graph_infers_transitive_impact() {
    archivum::DependencyGraph graph;
    graph.register_node(make_node(1, "Provider", {}));
    graph.register_node(make_node(2, "Consumer", {"Provider"}));
    graph.register_node(make_node(3, "Leaf", {"Consumer"}));
    graph.infer_dependencies();

    std::unordered_set<archivum::NodeId> radius = graph.calculate_blast_radius(1);
    require(radius.size() == 3, "transitive inferred impact is incomplete");
    require(graph.edge_count() == 2, "unexpected inferred edge count");
}

```
