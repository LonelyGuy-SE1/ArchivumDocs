# type_name

- Type: function
- Source: `src/main.cpp:168-181`
- Interface hash: `6466935875337316873`
- Source hash: `10743748983517377540`

## Signature

```cpp
std::string type_name(archivum::NodeType type)
```

## References

- `CLASS`
- `FUNCTION`
- `METHOD`
- `NodeType`
- `STRUCT`
- `archivum`
- `std`
- `string`
- `type`

## Source

```cpp
std::string type_name(archivum::NodeType type) {
    switch (type) {
        case archivum::NodeType::FUNCTION:
            return "function";
        case archivum::NodeType::CLASS:
            return "class";
        case archivum::NodeType::STRUCT:
            return "struct";
        case archivum::NodeType::METHOD:
            return "method";
        default:
            return "symbol";
    }
}

```
