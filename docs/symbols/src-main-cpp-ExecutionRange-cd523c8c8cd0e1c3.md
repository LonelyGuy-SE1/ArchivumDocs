# ExecutionRange

- Type: struct
- Source: `src/main.cpp:26-31`
- Interface hash: `8244194309201230723`
- Source hash: `11133390469397414407`

## Signature

```cpp
struct ExecutionRange
```

## References

- `base_sha`
- `head_sha`
- `initial_scan`
- `source`
- `std`
- `string`

## Source

```cpp
struct ExecutionRange {
    std::string base_sha;
    std::string head_sha;
    std::string source;
    bool initial_scan;
};

```
