---
layout: default
title: "Symbol: type_name"
---

# type_name

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:14-27` |
| **Interface Hash** | `4001346402825370292` |
| **Source Hash** | `5163732963069264905` |

## Signature

```cpp
std::string type_name(NodeType type)
```

## Dependencies

- `CLASS`
- `FUNCTION`
- `METHOD`
- `NodeType`
- `STRUCT`
- `std`
- `string`
- `type`

## Implementation

```cpp
std::string type_name(NodeType type) {
    switch (type) {
        case NodeType::FUNCTION:
            return "Function";
        case NodeType::CLASS:
            return "Class";
        case NodeType::STRUCT:
            return "Struct";
        case NodeType::METHOD:
            return "Method";
        default:
            return "Symbol";
    }
}

```
