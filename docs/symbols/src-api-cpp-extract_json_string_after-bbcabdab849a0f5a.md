# extract_json_string_after

- Type: function
- Source: `src/api.cpp:100-137`
- Interface hash: `11651946543704553940`
- Source hash: `4817527775060071935`

## Signature

```cpp
std::optional<std::string> extract_json_string_after(const std::string& body, size_t offset, const std::string& key)
```

## References

- `body`
- `c`
- `colon`
- `encoded`
- `escaped`
- `find`
- `i`
- `json_unescape`
- `key`
- `key_pos`
- `npos`
- `nullopt`
- `offset`
- `optional`
- `push_back`
- `quote`
- `size`
- `std`
- `string`

## Source

```cpp
std::optional<std::string> extract_json_string_after(const std::string& body, size_t offset, const std::string& key) {
    size_t key_pos = body.find(key, offset);
    if (key_pos == std::string::npos) {
        return std::nullopt;
    }

    size_t colon = body.find(':', key_pos + key.size());
    if (colon == std::string::npos) {
        return std::nullopt;
    }

    size_t quote = body.find('"', colon + 1);
    if (quote == std::string::npos) {
        return std::nullopt;
    }

    std::string encoded;
    bool escaped = false;
    for (size_t i = quote + 1; i < body.size(); ++i) {
        char c = body[i];
        if (escaped) {
            encoded.push_back('\\');
            encoded.push_back(c);
            escaped = false;
            continue;
        }
        if (c == '\\') {
            escaped = true;
            continue;
        }
        if (c == '"') {
            return json_unescape(encoded);
        }
        encoded.push_back(c);
    }

    return std::nullopt;
}

```
