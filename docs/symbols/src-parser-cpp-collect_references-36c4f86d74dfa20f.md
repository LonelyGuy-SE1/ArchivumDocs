# collect_references

- Type: function
- Source: `src/parser.cpp:166-179`
- Interface hash: `3126444816466565265`
- Source hash: `15060565652790768913`

## Signature

```cpp
void collect_references(TSNode node, const std::string& source_code, std::set<std::string>& references)
```

## References

- `TSNode`
- `child_count`
- `empty`
- `index`
- `insert`
- `is_identifier_node`
- `node`
- `references`
- `set`
- `simple_symbol`
- `source_code`
- `std`
- `string`
- `text_for_node`
- `ts_node_named_child`
- `ts_node_named_child_count`
- `ts_node_type`
- `type`
- `value`

## Source

```cpp
void collect_references(TSNode node, const std::string& source_code, std::set<std::string>& references) {
    std::string type = ts_node_type(node);
    if (is_identifier_node(type)) {
        std::string value = simple_symbol(text_for_node(node, source_code));
        if (!value.empty()) {
            references.insert(value);
        }
    }

    uint32_t child_count = ts_node_named_child_count(node);
    for (uint32_t index = 0; index < child_count; ++index) {
        collect_references(ts_node_named_child(node, index), source_code, references);
    }
}

```
