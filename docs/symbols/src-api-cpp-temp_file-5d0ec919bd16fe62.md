# temp_file

- Type: function
- Source: `src/api.cpp:156-159`
- Interface hash: `12214752897706943444`
- Source hash: `13385097007872472014`

## Signature

```cpp
std::filesystem::path temp_file(const std::string& name)
```

## References

- `chrono`
- `count`
- `filesystem`
- `name`
- `now`
- `path`
- `stamp`
- `std`
- `steady_clock`
- `string`
- `temp_directory_path`
- `time_since_epoch`
- `to_string`

## Source

```cpp
std::filesystem::path temp_file(const std::string& name) {
    auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() / ("archivum-" + name + "-" + std::to_string(stamp));
}

```
