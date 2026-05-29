# request_payload

- Type: function
- Source: `src/api.cpp:204-213`
- Interface hash: `17069801396756064696`
- Source hash: `13891836771199776909`

## Signature

```cpp
std::string request_payload(const ProviderRequest& request)
```

## References

- `ProviderRequest`
- `json_escape`
- `model`
- `ostringstream`
- `out`
- `prompt`
- `reasoning_effort`
- `request`
- `std`
- `str`
- `string`
- `verbosity`

## Source

```cpp
std::string request_payload(const ProviderRequest& request) {
    std::ostringstream out;
    out << "{";
    out << "\"model\":\"" << json_escape(request.model) << "\",";
    out << "\"input\":\"" << json_escape(request.prompt) << "\",";
    out << "\"reasoning\":{\"effort\":\"" << json_escape(request.reasoning_effort) << "\"},";
    out << "\"text\":{\"verbosity\":\"" << json_escape(request.verbosity) << "\"}";
    out << "}";
    return out.str();
}

```
