# simple_symbol

- Type: function
- Source: `src/parser.cpp:72-89`
- Interface hash: `8493691521077309200`
- Source hash: `9404699995129183195`

## Signature

```cpp
std::string simple_symbol(std::string value)
```

## References

- `back`
- `begin`
- `empty`
- `erase`
- `find`
- `front`
- `isalnum`
- `npos`
- `pop_back`
- `rfind`
- `scope`
- `static_cast`
- `std`
- `string`
- `substr`
- `template_start`
- `value`

## Source

```cpp
std::string simple_symbol(std::string value) {
    size_t template_start = value.find('<');
    if (template_start != std::string::npos) {
        value = value.substr(0, template_start);
    }
    size_t scope = value.rfind("::");
    if (scope != std::string::npos) {
        value = value.substr(scope + 2);
    }
    while (!value.empty() && !std::isalnum(static_cast<unsigned char>(value.front())) && value.front() != '_' &&
           value.front() != '~') {
        value.erase(value.begin());
    }
    while (!value.empty() && !std::isalnum(static_cast<unsigned char>(value.back())) && value.back() != '_') {
        value.pop_back();
    }
    return value;
}

```
