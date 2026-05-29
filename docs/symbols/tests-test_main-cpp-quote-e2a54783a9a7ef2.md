# quote

- Type: function
- Source: `tests/test_main.cpp:58-58`
- Interface hash: `18006776849801541055`
- Source hash: `6955354802874967035`

## Signature

```cpp
std::string quote(const std::filesystem::path& path)
```

## References

- `filesystem`
- `path`
- `std`
- `string`

## Source

```cpp
std::string quote(const std::filesystem::path& path) { return "\"" + path.string() + "\""; }

```
