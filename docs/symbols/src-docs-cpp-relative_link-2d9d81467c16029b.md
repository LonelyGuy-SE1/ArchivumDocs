---
layout: default
title: "Symbol: relative_link"
---

# relative_link

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:52-54` |
| **Interface Hash** | `17767781130707924402` |
| **Source Hash** | `10031664026895184680` |

## Signature

```cpp
std::string relative_link(const std::filesystem::path& from, const std::filesystem::path& to)
```

## Dependencies

- `filesystem`
- `from`
- `generic_string`
- `parent_path`
- `path`
- `relative`
- `std`
- `string`
- `to`

## Implementation

```cpp
std::string relative_link(const std::filesystem::path& from, const std::filesystem::path& to) {
    return std::filesystem::relative(to, from.parent_path()).generic_string();
}

```
