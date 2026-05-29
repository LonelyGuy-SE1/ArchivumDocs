# DocumentationWriteResult

- Type: struct
- Source: `include/archivum/docs.hpp:27-30`
- Interface hash: `1592849280504583628`
- Source hash: `15992026821765613072`

## Signature

```cpp
struct DocumentationWriteResult
```

## References

- `changed`
- `filesystem`
- `path`
- `std`
- `vector`
- `written_files`

## Source

```cpp
struct DocumentationWriteResult {
    bool changed = false;
    std::vector<std::filesystem::path> written_files;
};

```
