# parser_extracts_structural_metadata

- Type: function
- Source: `tests/test_main.cpp:85-119`
- Interface hash: `3950666333417907632`
- Source hash: `5752154789783609108`

## Signature

```cpp
void parser_extracts_structural_metadata()
```

## References

- `ASTParser`
- `CLASS`
- `FUNCTION`
- `METHOD`
- `Node`
- `NodeType`
- `STRUCT`
- `ScopedTemp`
- `archivum`
- `begin`
- `end`
- `filesystem`
- `find`
- `find_node`
- `helper`
- `id`
- `interface_hash`
- `nodes`
- `outer`
- `parse_file`
- `parser`
- `path`
- `payload`
- `references`
- `require`
- `sample`
- `second_outer`
- `second_pass`
- `source_hash`
- `std`
- `string`
- `temp`
- `type`
- `value`
- `vector`
- `widget`
- `write_file`

## Source

```cpp
void parser_extracts_structural_metadata() {
    ScopedTemp temp;
    std::filesystem::path sample = temp.path() / "sample.cpp";
    write_file(sample,
               "namespace alpha {\n"
               "struct Payload { int count; };\n"
               "int helper() { return 1; }\n"
               "class Widget {\n"
               "public:\n"
               "    int value() const { return helper(); }\n"
               "};\n"
               "int outer() { Widget widget; return widget.value(); }\n"
               "}\n");

    archivum::ASTParser parser;
    std::vector<archivum::Node> nodes = parser.parse_file(sample.string());

    const archivum::Node* payload = find_node(nodes, "Payload");
    const archivum::Node* helper = find_node(nodes, "helper");
    const archivum::Node* widget = find_node(nodes, "Widget");
    const archivum::Node* value = find_node(nodes, "value");
    const archivum::Node* outer = find_node(nodes, "outer");

    require(payload != nullptr && payload->type == archivum::NodeType::STRUCT, "struct not extracted");
    require(helper != nullptr && helper->type == archivum::NodeType::FUNCTION, "function not extracted");
    require(widget != nullptr && widget->type == archivum::NodeType::CLASS, "class not extracted");
    require(value != nullptr && value->type == archivum::NodeType::METHOD, "method not extracted");
    require(outer != nullptr && outer->interface_hash != 0 && outer->source_hash != 0, "function hashes missing");
    require(std::find(outer->references.begin(), outer->references.end(), "Widget") != outer->references.end(),
            "references missing class use");

    std::vector<archivum::Node> second_pass = parser.parse_file(sample.string());
    const archivum::Node* second_outer = find_node(second_pass, "outer");
    require(second_outer != nullptr && second_outer->id == outer->id, "node id is not stable");
}

```
