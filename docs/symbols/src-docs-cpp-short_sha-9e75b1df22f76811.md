---
layout: default
title: "Symbol: short_sha"
---

# short_sha

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:50-50` |
| **Interface Hash** | `2243655815905749264` |
| **Source Hash** | `2847802328774561850` |

## Signature

```cpp
std::string short_sha(const std::string& sha)
```

## Dependencies

- `sha`
- `size`
- `std`
- `string`
- `substr`

## Implementation

```cpp
std::string short_sha(const std::string& sha) { return sha.size() > 8 ? sha.substr(0, 8) : sha; }

```
