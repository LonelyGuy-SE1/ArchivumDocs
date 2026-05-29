# interface_text

- Type: function
- Source: `src/parser.cpp:145-164`
- Interface hash: `12612800021564858312`
- Source hash: `124486532404456300`

## Signature

```cpp
std::string interface_text(TSNode node, const std::string& source_code)
```

## References

- `TSNode`
- `body`
- `brace`
- `end`
- `find`
- `node`
- `normalize_space`
- `npos`
- `size`
- `source_code`
- `start`
- `static_cast`
- `std`
- `string`
- `substr`
- `ts_node_child_by_field_name`
- `ts_node_end_byte`
- `ts_node_is_null`
- `ts_node_start_byte`

## Source

```cpp
std::string interface_text(TSNode node, const std::string& source_code) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);

    TSNode body = ts_node_child_by_field_name(node, "body", 4);
    if (!ts_node_is_null(body)) {
        end = ts_node_start_byte(body);
    } else {
        size_t brace = source_code.find('{', start);
        if (brace != std::string::npos && brace < end) {
            end = static_cast<uint32_t>(brace);
        }
    }

    if (start > end || end > source_code.size()) {
        return {};
    }

    return normalize_space(source_code.substr(start, end - start));
}

```
