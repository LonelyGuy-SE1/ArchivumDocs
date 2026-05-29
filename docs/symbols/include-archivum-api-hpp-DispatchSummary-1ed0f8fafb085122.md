# DispatchSummary

- Type: struct
- Source: `include/archivum/api.hpp:11-17`
- Interface hash: `234153944208463214`
- Source hash: `277206421778780009`

## Signature

```cpp
struct DispatchSummary
```

## References

- `context_symbols`
- `credentials_available`
- `downstream_symbols`
- `mutated_symbols`
- `provider`
- `std`
- `string`

## Source

```cpp
struct DispatchSummary {
    std::string provider;
    bool credentials_available;
    size_t mutated_symbols;
    size_t downstream_symbols;
    size_t context_symbols;
};

```
