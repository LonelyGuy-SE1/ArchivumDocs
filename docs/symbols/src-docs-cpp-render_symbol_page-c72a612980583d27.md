# render_symbol_page

- Type: function
- Source: `src/docs.cpp:112-138`
- Interface hash: `13319630199632855732`
- Source hash: `789825869120612670`

## Signature

```cpp
std::string render_symbol_page(const Node& node, const std::string& source)
```

## References

- `Node`
- `empty`
- `end_line`
- `file_path`
- `interface_hash`
- `name`
- `node`
- `ostringstream`
- `out`
- `reference`
- `references`
- `signature`
- `source`
- `source_hash`
- `start_line`
- `std`
- `str`
- `string`
- `type`
- `type_name`

## Source

```cpp
std::string render_symbol_page(const Node& node, const std::string& source) {
    std::ostringstream out;
    out << "# " << node.name << "\n\n";
    out << "- Type: " << type_name(node.type) << "\n";
    out << "- Source: `" << node.file_path << ":" << node.start_line << "-" << node.end_line << "`\n";
    out << "- Interface hash: `" << node.interface_hash << "`\n";
    out << "- Source hash: `" << node.source_hash << "`\n";

    if (!node.signature.empty()) {
        out << "\n## Signature\n\n";
        out << "```cpp\n" << node.signature << "\n```\n";
    }

    if (!node.references.empty()) {
        out << "\n## References\n\n";
        for (const std::string& reference : node.references) {
            out << "- `" << reference << "`\n";
        }
    }

    if (!source.empty()) {
        out << "\n## Source\n\n";
        out << "```cpp\n" << source << "\n```\n";
    }

    return out.str();
}

```
