# extract_response_text

- Type: function
- Source: `src/api.cpp:139-154`
- Interface hash: `5476954390551206599`
- Source hash: `4383882977864991424`

## Signature

```cpp
std::optional<std::string> extract_response_text(const std::string& body)
```

## References

- `body`
- `extract_json_string_after`
- `find`
- `has_value`
- `message_content`
- `npos`
- `nullopt`
- `optional`
- `output_text`
- `std`
- `string`
- `text`

## Source

```cpp
std::optional<std::string> extract_response_text(const std::string& body) {
    size_t output_text = body.find("\"output_text\"");
    if (output_text != std::string::npos) {
        std::optional<std::string> text = extract_json_string_after(body, output_text, "\"text\"");
        if (text.has_value()) {
            return text;
        }
    }

    size_t message_content = body.find("\"content\"");
    if (message_content != std::string::npos) {
        return extract_json_string_after(body, message_content, "\"content\"");
    }

    return std::nullopt;
}

```
