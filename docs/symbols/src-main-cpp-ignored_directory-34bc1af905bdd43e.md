# ignored_directory

- Type: function
- Source: `src/main.cpp:56-59`
- Interface hash: `2133007221572503311`
- Source hash: `8626729349743918957`

## Signature

```cpp
bool ignored_directory(const std::filesystem::path& path)
```

## References

- `filename`
- `filesystem`
- `name`
- `path`
- `std`
- `string`

## Source

```cpp
bool ignored_directory(const std::filesystem::path& path) {
    std::string name = path.filename().string();
    return name == ".git" || name == ".github" || name == ".vscode" || name == "build" || name == "docs";
}

```
