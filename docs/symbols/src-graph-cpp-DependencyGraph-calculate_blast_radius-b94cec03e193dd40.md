# DependencyGraph::calculate_blast_radius

- Type: function
- Source: `src/graph.cpp:61-88`
- Interface hash: `4309191442389237414`
- Source hash: `11255699224691159190`

## Signature

```cpp
std::unordered_set<NodeId> DependencyGraph::calculate_blast_radius(NodeId origin_id) const
```

## References

- `DependencyGraph`
- `NodeId`
- `blast_radius`
- `consumer`
- `current`
- `downstream_edges`
- `edge_it`
- `empty`
- `end`
- `find`
- `front`
- `frontier`
- `insert`
- `origin_id`
- `pop`
- `push`
- `queue`
- `registry`
- `second`
- `std`
- `unordered_set`

## Source

```cpp
std::unordered_set<NodeId> DependencyGraph::calculate_blast_radius(NodeId origin_id) const {
    std::unordered_set<NodeId> blast_radius;

    if (registry.find(origin_id) == registry.end()) {
        return blast_radius;
    }

    std::queue<NodeId> frontier;
    frontier.push(origin_id);
    blast_radius.insert(origin_id);

    while (!frontier.empty()) {
        NodeId current = frontier.front();
        frontier.pop();

        auto edge_it = downstream_edges.find(current);
        if (edge_it != downstream_edges.end()) {
            for (NodeId consumer : edge_it->second) {
                if (blast_radius.find(consumer) == blast_radius.end()) {
                    blast_radius.insert(consumer);
                    frontier.push(consumer);
                }
            }
        }
    }

    return blast_radius;
}

```
