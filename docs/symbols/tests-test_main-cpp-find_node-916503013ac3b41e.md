# find_node

- Type: function
- Source: `tests/test_main.cpp:80-83`
- Interface hash: `15596686308944566493`
- Source hash: `6275627977899750430`

## Signature

```cpp
const archivum::Node* find_node(const std::vector<archivum::Node>& nodes, const std::string& name)
```

## References

- `Node`
- `archivum`
- `begin`
- `end`
- `find_if`
- `it`
- `name`
- `node`
- `nodes`
- `std`
- `string`
- `vector`

## Source

```cpp
const archivum::Node* find_node(const std::vector<archivum::Node>& nodes, const std::string& name) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const archivum::Node& node) { return node.name == name; });
    return it == nodes.end() ? nullptr : &*it;
}

```
