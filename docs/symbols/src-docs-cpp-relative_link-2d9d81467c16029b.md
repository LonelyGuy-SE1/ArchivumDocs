# relative_link

- Type: function
- Source: `src/docs.cpp:53-55`
- Interface hash: `17767781130707924402`
- Source hash: `10031664026895184680`

## Signature

```cpp
std::string relative_link(const std::filesystem::path& from, const std::filesystem::path& to)
```

## References

- `filesystem`
- `from`
- `generic_string`
- `parent_path`
- `path`
- `relative`
- `std`
- `string`
- `to`

## Source

```cpp
std::string relative_link(const std::filesystem::path& from, const std::filesystem::path& to) {
    return std::filesystem::relative(to, from.parent_path()).generic_string();
}

```
