# ASTParser::generate_node_id

- Type: function
- Source: `src/parser.cpp:196-199`
- Interface hash: `18169083682321222398`
- Source hash: `17063645151975798419`

## Signature

```cpp
uint64_t ASTParser::generate_node_id(const std::string& filepath, const std::string& symbol_name) const
```

## References

- `ASTParser`
- `composite_key`
- `filepath`
- `stable_hash`
- `std`
- `string`
- `symbol_name`

## Source

```cpp
uint64_t ASTParser::generate_node_id(const std::string& filepath, const std::string& symbol_name) const {
    std::string composite_key = filepath + "::" + symbol_name;
    return stable_hash(composite_key);
}

```
