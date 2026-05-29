# symbol_filename

- Type: function
- Source: `src/docs.cpp:57-61`
- Interface hash: `6651250992335654481`
- Source hash: `5397213886734098262`

## Signature

```cpp
std::string symbol_filename(const Node& node)
```

## References

- `Node`
- `file_path`
- `hex`
- `id`
- `name`
- `node`
- `ostringstream`
- `out`
- `sanitize`
- `std`
- `str`
- `string`

## Source

```cpp
std::string symbol_filename(const Node& node) {
    std::ostringstream out;
    out << sanitize(node.file_path) << "-" << sanitize(node.name) << "-" << std::hex << node.id << ".md";
    return out.str();
}

```
