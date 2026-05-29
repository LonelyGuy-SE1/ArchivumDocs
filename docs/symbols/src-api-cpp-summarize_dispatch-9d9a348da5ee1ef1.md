# summarize_dispatch

- Type: function
- Source: `src/api.cpp:217-233`
- Interface hash: `6518633790942445563`
- Source hash: `8236682554649986654`

## Signature

```cpp
DispatchSummary summarize_dispatch(const DependencyGraph& graph, const std::unordered_set<NodeId>& mutated, const std::unordered_set<NodeId>& impacted)
```

## References

- `DependencyGraph`
- `DispatchSummary`
- `NodeId`
- `contains`
- `context_symbols`
- `credentials_available`
- `downstream_symbols`
- `graph`
- `id`
- `impacted`
- `mutated`
- `mutated_symbols`
- `provider`
- `provider_name`
- `size`
- `std`
- `summary`
- `unordered_set`

## Source

```cpp
DispatchSummary summarize_dispatch(const DependencyGraph& graph, const std::unordered_set<NodeId>& mutated,
                                   const std::unordered_set<NodeId>& impacted) {
    DispatchSummary summary;
    summary.provider = provider_name();
    summary.credentials_available = summary.provider != "shadow";
    summary.mutated_symbols = mutated.size();
    summary.downstream_symbols = impacted.size() > mutated.size() ? impacted.size() - mutated.size() : 0;
    summary.context_symbols = 0;

    for (NodeId id : impacted) {
        if (graph.contains(id)) {
            ++summary.context_symbols;
        }
    }

    return summary;
}

```
