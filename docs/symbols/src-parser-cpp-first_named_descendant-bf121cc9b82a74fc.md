# first_named_descendant

- Type: function
- Source: `src/parser.cpp:91-120`
- Interface hash: `5693522910223469499`
- Source hash: `13060712392378425656`

## Signature

```cpp
std::string first_named_descendant(TSNode node, const std::string& source_code)
```

## References

- `TSNode`
- `char_traits`
- `child`
- `child_count`
- `empty`
- `field`
- `fields`
- `index`
- `is_identifier_node`
- `length`
- `node`
- `source_code`
- `static_cast`
- `std`
- `string`
- `text_for_node`
- `ts_node_child_by_field_name`
- `ts_node_is_null`
- `ts_node_named_child`
- `ts_node_named_child_count`
- `ts_node_type`
- `type`
- `value`

## Source

```cpp
std::string first_named_descendant(TSNode node, const std::string& source_code) {
    if (ts_node_is_null(node)) {
        return {};
    }

    std::string type = ts_node_type(node);
    if (is_identifier_node(type)) {
        return text_for_node(node, source_code);
    }

    static constexpr const char* fields[] = {"declarator", "name"};
    for (const char* field : fields) {
        TSNode child =
            ts_node_child_by_field_name(node, field, static_cast<uint32_t>(std::char_traits<char>::length(field)));
        std::string value = first_named_descendant(child, source_code);
        if (!value.empty()) {
            return value;
        }
    }

    uint32_t child_count = ts_node_named_child_count(node);
    for (uint32_t index = 0; index < child_count; ++index) {
        std::string value = first_named_descendant(ts_node_named_child(node, index), source_code);
        if (!value.empty()) {
            return value;
        }
    }

    return {};
}

```
