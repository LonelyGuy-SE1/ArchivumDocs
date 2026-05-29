# DependencyGraph::infer_dependencies

- Type: function
- Source: `src/graph.cpp:31-59`
- Interface hash: `4092975540515250063`
- Source hash: `7444074456455261555`

## Signature

```cpp
void DependencyGraph::infer_dependencies()
```

## References

- `DependencyGraph`
- `NodeId`
- `clear`
- `consumer`
- `consumer_id`
- `downstream_edges`
- `end`
- `find`
- `id`
- `insert`
- `it`
- `linked`
- `name`
- `node`
- `provider_id`
- `providers`
- `push_back`
- `reference`
- `references`
- `registry`
- `reserve`
- `second`
- `size`
- `std`
- `string`
- `symbol_key`
- `unordered_map`
- `unordered_set`
- `vector`

## Source

```cpp
void DependencyGraph::infer_dependencies() {
    downstream_edges.clear();

    std::unordered_map<std::string, std::vector<NodeId>> providers;
    providers.reserve(registry.size() * 2);

    for (const auto& [id, node] : registry) {
        providers[node.name].push_back(id);
        providers[symbol_key(node.name)].push_back(id);
    }

    for (const auto& [consumer_id, consumer] : registry) {
        std::unordered_set<NodeId> linked;
        for (const std::string& reference : consumer.references) {
            auto it = providers.find(symbol_key(reference));
            if (it == providers.end()) {
                it = providers.find(reference);
            }
            if (it == providers.end()) {
                continue;
            }
            for (NodeId provider_id : it->second) {
                if (provider_id != consumer_id && linked.insert(provider_id).second) {
                    downstream_edges[provider_id].insert(consumer_id);
                }
            }
        }
    }
}

```
