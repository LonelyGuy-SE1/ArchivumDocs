# is_source_file

- Type: function
- Source: `src/main.cpp:47-54`
- Interface hash: `9179527729873657494`
- Source hash: `10462796219723122284`

## Signature

```cpp
bool is_source_file(const std::filesystem::path& path)
```

## References

- `begin`
- `c`
- `end`
- `extension`
- `filesystem`
- `path`
- `static_cast`
- `std`
- `string`
- `tolower`
- `transform`

## Source

```cpp
bool is_source_file(const std::filesystem::path& path) {
    std::string extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    return extension == ".c" || extension == ".cc" || extension == ".cpp" || extension == ".cxx" || extension == ".h" ||
           extension == ".hh" || extension == ".hpp" || extension == ".hxx" || extension == ".ipp";
}

```
