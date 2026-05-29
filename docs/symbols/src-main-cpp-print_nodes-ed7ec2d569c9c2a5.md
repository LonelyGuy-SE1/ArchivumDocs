---
layout: default
title: "Symbol: print_nodes"
---

# print_nodes

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/main.cpp:205-218` |
| **Interface Hash** | `15257581353757047444` |
| **Source Hash** | `7321665852407165890` |

## Signature

```cpp
void print_nodes(const std::string& title, const std::vector<archivum::Node>& nodes, size_t limit)
```

## Dependencies

- `Node`
- `archivum`
- `cout`
- `end_line`
- `file_path`
- `index`
- `limit`
- `min`
- `name`
- `node`
- `nodes`
- `shown`
- `size`
- `start_line`
- `std`
- `string`
- `title`
- `type`
- `type_name`
- `vector`

## Implementation

```cpp
void print_nodes(const std::string& title, const std::vector<archivum::Node>& nodes, size_t limit) {
    std::cout << "[ArchivumDocs] " << title << ": " << nodes.size() << "\n";

    size_t shown = std::min(limit, nodes.size());
    for (size_t index = 0; index < shown; ++index) {
        const archivum::Node& node = nodes[index];
        std::cout << "  - " << type_name(node.type) << " " << node.name << " [" << node.file_path << ":"
                  << node.start_line << "-" << node.end_line << "]\n";
    }

    if (nodes.size() > shown) {
        std::cout << "  - +" << (nodes.size() - shown) << " more\n";
    }
}

```
