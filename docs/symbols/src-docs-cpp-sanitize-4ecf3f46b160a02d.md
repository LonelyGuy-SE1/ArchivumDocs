---
layout: default
title: "Symbol: sanitize"
---

# sanitize

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:29-48` |
| **Interface Hash** | `11062865004325912250` |
| **Source Hash** | `1625117893506851092` |

## Signature

```cpp
std::string sanitize(std::string value)
```

## Dependencies

- `back`
- `begin`
- `c`
- `empty`
- `end`
- `erase`
- `front`
- `isalnum`
- `keep`
- `left`
- `pop_back`
- `right`
- `static_cast`
- `std`
- `string`
- `unique`
- `value`

## Implementation

```cpp
std::string sanitize(std::string value) {
    for (char& c : value) {
        bool keep = std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_';
        if (!keep) {
            c = '-';
        }
    }

    value.erase(
        std::unique(value.begin(), value.end(), [](char left, char right) { return left == '-' && right == '-'; }),
        value.end());

    while (!value.empty() && value.front() == '-') {
        value.erase(value.begin());
    }
    while (!value.empty() && value.back() == '-') {
        value.pop_back();
    }
    return value.empty() ? "symbol" : value;
}

```
