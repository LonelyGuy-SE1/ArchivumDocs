# structural_name

- Type: function
- Source: `src/parser.cpp:122-131`
- Interface hash: `4091368069231774474`
- Source hash: `17143463682800517994`

## Signature

```cpp
std::string structural_name(TSNode node, const std::string& source_code)
```

## References

- `TSNode`
- `declarator`
- `first_named_descendant`
- `name`
- `node`
- `source_code`
- `std`
- `string`
- `text_for_node`
- `ts_node_child_by_field_name`
- `ts_node_type`
- `type`

## Source

```cpp
std::string structural_name(TSNode node, const std::string& source_code) {
    std::string type = ts_node_type(node);
    if (type == "class_specifier" || type == "struct_specifier") {
        TSNode name = ts_node_child_by_field_name(node, "name", 4);
        return text_for_node(name, source_code);
    }

    TSNode declarator = ts_node_child_by_field_name(node, "declarator", 10);
    return first_named_descendant(declarator, source_code);
}

```
