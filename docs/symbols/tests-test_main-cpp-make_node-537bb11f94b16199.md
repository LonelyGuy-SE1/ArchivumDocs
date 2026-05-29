# make_node

- Type: function
- Source: `tests/test_main.cpp:65-78`
- Interface hash: `3921473017631630789`
- Source hash: `5796579956993490125`

## Signature

```cpp
archivum::Node make_node(archivum::NodeId id, const std::string& name, std::vector<std::string> references)
```

## References

- `FUNCTION`
- `Node`
- `NodeId`
- `NodeType`
- `archivum`
- `end_line`
- `file_path`
- `id`
- `interface_hash`
- `move`
- `name`
- `node`
- `references`
- `signature`
- `source_hash`
- `start_line`
- `static_cast`
- `std`
- `string`
- `type`
- `vector`

## Source

```cpp
archivum::Node make_node(archivum::NodeId id, const std::string& name, std::vector<std::string> references) {
    archivum::Node node;
    node.id = id;
    node.name = name;
    node.file_path = "unit.cpp";
    node.signature = name;
    node.references = std::move(references);
    node.start_line = static_cast<uint32_t>(id);
    node.end_line = static_cast<uint32_t>(id);
    node.type = archivum::NodeType::FUNCTION;
    node.interface_hash = id * 17;
    node.source_hash = id * 19;
    return node;
}

```
