# normalize_space

- Type: function
- Source: `src/parser.cpp:41-63`
- Interface hash: `4719072263180046919`
- Source hash: `4940521656853076`

## Signature

```cpp
std::string normalize_space(std::string value)
```

## References

- `back`
- `c`
- `empty`
- `isspace`
- `pending_space`
- `pop_back`
- `push_back`
- `reserve`
- `result`
- `size`
- `static_cast`
- `std`
- `string`
- `value`

## Source

```cpp
std::string normalize_space(std::string value) {
    std::string result;
    result.reserve(value.size());
    bool pending_space = false;

    for (unsigned char c : value) {
        if (std::isspace(c)) {
            pending_space = !result.empty();
            continue;
        }
        if (pending_space && c != ';' && c != ',' && c != ')' && c != '(' && c != '{') {
            result.push_back(' ');
        }
        pending_space = false;
        result.push_back(static_cast<char>(c));
    }

    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.back()))) {
        result.pop_back();
    }

    return result;
}

```
