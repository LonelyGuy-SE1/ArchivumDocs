# write_file

- Type: function
- Source: `tests/test_main.cpp:51-56`
- Interface hash: `3046141197959230417`
- Source hash: `9073578338190526048`

## Signature

```cpp
void write_file(const std::filesystem::path& path, const std::string& content)
```

## References

- `binary`
- `content`
- `create_directories`
- `filesystem`
- `good`
- `ios`
- `ofstream`
- `out`
- `parent_path`
- `path`
- `require`
- `std`
- `string`

## Source

```cpp
void write_file(const std::filesystem::path& path, const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    require(out.good(), "failed to open " + path.string());
    out << content;
}

```
