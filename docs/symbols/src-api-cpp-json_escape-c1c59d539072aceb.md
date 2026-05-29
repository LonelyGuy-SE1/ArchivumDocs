# json_escape

- Type: function
- Source: `src/api.cpp:31-62`
- Interface hash: `12253582445087056658`
- Source hash: `5983866315728745297`

## Signature

```cpp
std::string json_escape(const std::string& value)
```

## References

- `c`
- `hex`
- `ostringstream`
- `out`
- `static_cast`
- `std`
- `str`
- `string`
- `value`

## Source

```cpp
std::string json_escape(const std::string& value) {
    std::ostringstream out;
    for (unsigned char c : value) {
        switch (c) {
            case '\\':
                out << "\\\\";
                break;
            case '"':
                out << "\\\"";
                break;
            case '\n':
                out << "\\n";
                break;
            case '\r':
                out << "\\r";
                break;
            case '\t':
                out << "\\t";
                break;
            default:
                if (c < 0x20) {
                    out << "\\u";
                    out << "00";
                    constexpr char hex[] = "0123456789abcdef";
                    out << hex[(c >> 4) & 0x0f] << hex[c & 0x0f];
                } else {
                    out << static_cast<char>(c);
                }
        }
    }
    return out.str();
}

```
