# symbol_key

- Type: function
- Source: `src/graph.cpp:10-18`
- Interface hash: `11729513786155181400`
- Source hash: `10654047079910515689`

## Signature

```cpp
std::string symbol_key(const std::string& value)
```

## References

- `find`
- `npos`
- `rfind`
- `scope`
- `std`
- `string`
- `substr`
- `template_start`
- `trimmed`
- `value`

## Source

```cpp
std::string symbol_key(const std::string& value) {
    size_t template_start = value.find('<');
    std::string trimmed = value.substr(0, template_start);
    size_t scope = trimmed.rfind("::");
    if (scope != std::string::npos) {
        return trimmed.substr(scope + 2);
    }
    return trimmed;
}

```
