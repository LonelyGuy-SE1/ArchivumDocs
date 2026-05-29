---
layout: default
title: "Symbol: symbol_filename"
---

# symbol_filename

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:56-60` |
| **Interface Hash** | `6651250992335654481` |
| **Source Hash** | `5397213886734098262` |

## Signature

```cpp
std::string symbol_filename(const Node& node)
```

## Dependencies

- `Node`
- `file_path`
- `hex`
- `id`
- `name`
- `node`
- `ostringstream`
- `out`
- `sanitize`
- `std`
- `str`
- `string`

## Implementation

```cpp
std::string symbol_filename(const Node& node) {
    std::ostringstream out;
    out << sanitize(node.file_path) << "-" << sanitize(node.name) << "-" << std::hex << node.id << ".md";
    return out.str();
}

```
