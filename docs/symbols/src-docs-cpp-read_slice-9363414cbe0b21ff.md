# read_slice

- Type: function
- Source: `src/docs.cpp:63-94`
- Interface hash: `3503476949664482557`
- Source hash: `17298385749232474804`

## Signature

```cpp
std::string read_slice(const std::filesystem::path& root, const Node& node, size_t limit)
```

## References

- `Node`
- `end_line`
- `file`
- `file_path`
- `filesystem`
- `getline`
- `ifstream`
- `is_open`
- `limit`
- `line`
- `line_number`
- `node`
- `ostringstream`
- `out`
- `path`
- `remaining`
- `root`
- `size`
- `start_line`
- `std`
- `str`
- `string`
- `substr`
- `written`

## Source

```cpp
std::string read_slice(const std::filesystem::path& root, const Node& node, size_t limit) {
    std::ifstream file(root / node.file_path);
    if (!file.is_open()) {
        return {};
    }

    std::ostringstream out;
    std::string line;
    uint32_t line_number = 0;
    size_t written = 0;

    while (std::getline(file, line)) {
        ++line_number;
        if (line_number < node.start_line) {
            continue;
        }
        if (line_number > node.end_line || written >= limit) {
            break;
        }

        size_t remaining = limit - written;
        if (line.size() + 1 > remaining) {
            out << line.substr(0, remaining);
            written = limit;
        } else {
            out << line << "\n";
            written += line.size() + 1;
        }
    }

    return out.str();
}

```
