# ASTParser::parse_file

- Type: function
- Source: `src/parser.cpp:201-234`
- Interface hash: `412891281206381901`
- Source hash: `2336044200298334151`

## Signature

```cpp
std::vector<Node> ASTParser::parse_file(const std::string& filepath)
```

## References

- `ASTParser`
- `Node`
- `TSNode`
- `TSTree`
- `buffer`
- `c_str`
- `cerr`
- `endl`
- `extract_structural_nodes`
- `extracted_nodes`
- `file`
- `filepath`
- `ifstream`
- `is_open`
- `max`
- `numeric_limits`
- `parser`
- `rdbuf`
- `root_node`
- `size`
- `source_code`
- `static_cast`
- `std`
- `str`
- `string`
- `stringstream`
- `tree`
- `ts_parser_parse_string`
- `ts_tree_delete`
- `ts_tree_root_node`
- `vector`

## Source

```cpp
std::vector<Node> ASTParser::parse_file(const std::string& filepath) {
    std::vector<Node> extracted_nodes;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[ArchivumDocs] ERROR: Could not open file: " << filepath << std::endl;
        return extracted_nodes;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source_code = buffer.str();

    if (source_code.size() > std::numeric_limits<uint32_t>::max()) {
        std::cerr << "[ArchivumDocs] ERROR: File is too large for tree-sitter: " << filepath << std::endl;
        return extracted_nodes;
    }

    TSTree* tree =
        ts_parser_parse_string(parser, nullptr, source_code.c_str(), static_cast<uint32_t>(source_code.size()));

    if (!tree) {
        std::cerr << "[ArchivumDocs] FATAL: Tree-sitter failed to parse file." << std::endl;
        return extracted_nodes;
    }

    TSNode root_node = ts_tree_root_node(tree);

    extract_structural_nodes(root_node, extracted_nodes, filepath, source_code);

    ts_tree_delete(tree);

    return extracted_nodes;
}

```
