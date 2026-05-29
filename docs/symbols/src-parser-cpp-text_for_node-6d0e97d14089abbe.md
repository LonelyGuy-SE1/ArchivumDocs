# text_for_node

- Type: function
- Source: `src/parser.cpp:29-39`
- Interface hash: `15590630846259607551`
- Source hash: `6016149031461640626`

## Signature

```cpp
std::string text_for_node(TSNode node, const std::string& source_code)
```

## References

- `TSNode`
- `end`
- `node`
- `size`
- `source_code`
- `start`
- `std`
- `string`
- `substr`
- `ts_node_end_byte`
- `ts_node_is_null`
- `ts_node_start_byte`

## Source

```cpp
std::string text_for_node(TSNode node, const std::string& source_code) {
    if (ts_node_is_null(node)) {
        return {};
    }
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    if (start > end || end > source_code.size()) {
        return {};
    }
    return source_code.substr(start, end - start);
}

```
