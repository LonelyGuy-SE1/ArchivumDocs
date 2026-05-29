---
layout: default
title: "Symbol: render_symbol_page"
---

# render_symbol_page

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:111-146` |
| **Interface Hash** | `13319630199632855732` |
| **Source Hash** | `15465518283548782265` |

## Signature

```cpp
std::string render_symbol_page(const Node& node, const std::string& source)
```

## Dependencies

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

## Implementation

```cpp
std::string render_symbol_page(const Node& node, const std::string& source) {
    std::ostringstream out;
    out << "---\n";
    out << "layout: default\n";
    out << "title: \"Symbol: " << node.name << "\"\n";
    out << "---\n\n";

    out << "# " << node.name << "\n\n";

    out << "| Metadata | Value |\n";
    out << "| :--- | :--- |\n";
    out << "| **Type** | " << type_name(node.type) << " |\n";
    out << "| **Location** | `" << node.file_path << ":" << node.start_line << "-" << node.end_line << "` |\n";
    out << "| **Interface Hash** | `" << node.interface_hash << "` |\n";
    out << "| **Source Hash** | `" << node.source_hash << "` |\n\n";

    if (!node.signature.empty()) {
        out << "## Signature\n\n";
        out << "```cpp\n" << node.signature << "\n```\n\n";
    }

    if (!node.references.empty()) {
        out << "## Dependencies\n\n";
        for (const std::string& reference : node.references) {
            out << "- `" << reference << "`\n";
        }
        out << "\n";
    }

    if (!source.empty()) {
        out << "## Implementation\n\n";
        out << "```cpp\n" << source << "\n```\n";
    }

    return out.str();
}

```
