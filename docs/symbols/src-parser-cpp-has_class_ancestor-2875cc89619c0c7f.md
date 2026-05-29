# has_class_ancestor

- Type: function
- Source: `src/parser.cpp:133-143`
- Interface hash: `11663133056075175604`
- Source hash: `6441557933142382322`

## Signature

```cpp
bool has_class_ancestor(TSNode node)
```

## References

- `TSNode`
- `node`
- `parent`
- `std`
- `string`
- `ts_node_is_null`
- `ts_node_parent`
- `ts_node_type`
- `type`

## Source

```cpp
bool has_class_ancestor(TSNode node) {
    TSNode parent = ts_node_parent(node);
    while (!ts_node_is_null(parent)) {
        std::string type = ts_node_type(parent);
        if (type == "class_specifier" || type == "struct_specifier") {
            return true;
        }
        parent = ts_node_parent(parent);
    }
    return false;
}

```
