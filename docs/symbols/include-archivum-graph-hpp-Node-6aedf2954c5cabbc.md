# Node

- Type: struct
- Source: `include/archivum/graph.hpp:15-28`
- Interface hash: `1410235776362031852`
- Source hash: `923427695967715332`

## Signature

```cpp
struct Node
```

## References

- `NodeId`
- `NodeType`
- `end_line`
- `file_path`
- `id`
- `interface_hash`
- `name`
- `references`
- `signature`
- `source_hash`
- `start_line`
- `std`
- `string`
- `type`
- `vector`

## Source

```cpp
struct Node {
    NodeId id;
    std::string name;
    std::string file_path;
    std::string signature;
    std::vector<std::string> references;

    uint32_t start_line;
    uint32_t end_line;
    NodeType type;

    uint64_t interface_hash;
    uint64_t source_hash;
};

```
