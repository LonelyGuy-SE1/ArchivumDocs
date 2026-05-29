# shell_quote

- Type: function
- Source: `src/api.cpp:161-174`
- Interface hash: `2196442106142709904`
- Source hash: `4574077958770157424`

## Signature

```cpp
std::string shell_quote(const std::filesystem::path& path)
```

## References

- `c`
- `filesystem`
- `path`
- `push_back`
- `quoted`
- `std`
- `string`
- `value`

## Source

```cpp
std::string shell_quote(const std::filesystem::path& path) {
    std::string value = path.string();
    std::string quoted = "\"";
    for (char c : value) {
        if (c == '"' || c == '\\') {
            quoted.push_back('\\');
            quoted.push_back(c);
        } else {
            quoted.push_back(c);
        }
    }
    quoted.push_back('"');
    return quoted;
}

```
