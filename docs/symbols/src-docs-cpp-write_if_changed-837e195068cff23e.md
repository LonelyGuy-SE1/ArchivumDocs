# write_if_changed

- Type: function
- Source: `src/docs.cpp:96-110`
- Interface hash: `7952235644962819281`
- Source hash: `6760561587945754461`

## Signature

```cpp
bool write_if_changed(const std::filesystem::path& path, const std::string& content)
```

## References

- `binary`
- `buffer`
- `content`
- `create_directories`
- `existing`
- `filesystem`
- `ifstream`
- `ios`
- `is_open`
- `ofstream`
- `ostringstream`
- `out`
- `parent_path`
- `path`
- `rdbuf`
- `std`
- `str`
- `string`

## Source

```cpp
bool write_if_changed(const std::filesystem::path& path, const std::string& content) {
    std::ifstream existing(path, std::ios::binary);
    if (existing.is_open()) {
        std::ostringstream buffer;
        buffer << existing.rdbuf();
        if (buffer.str() == content) {
            return false;
        }
    }

    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    out << content;
    return true;
}

```
