# is_identifier_node

- Type: function
- Source: `src/parser.cpp:65-70`
- Interface hash: `5341187788475723923`
- Source hash: `6216472343893109419`

## Signature

```cpp
bool is_identifier_node(std::string_view type)
```

## References

- `std`
- `string_view`
- `type`

## Source

```cpp
bool is_identifier_node(std::string_view type) {
    return type == "identifier" || type == "field_identifier" || type == "type_identifier" ||
           type == "namespace_identifier" || type == "qualified_identifier" || type == "scoped_identifier" ||
           type == "destructor_name" || type == "operator_name" || type == "template_type" ||
           type == "template_function";
}

```
