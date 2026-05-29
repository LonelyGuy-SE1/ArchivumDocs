# ProviderRequest

- Type: struct
- Source: `include/archivum/api.hpp:19-28`
- Interface hash: `14310645714837441786`
- Source hash: `14528147521462235130`

## Signature

```cpp
struct ProviderRequest
```

## References

- `api_key_env`
- `endpoint`
- `fail_on_error`
- `model`
- `prompt`
- `provider`
- `reasoning_effort`
- `std`
- `string`
- `verbosity`

## Source

```cpp
struct ProviderRequest {
    std::string provider;
    std::string endpoint;
    std::string model;
    std::string api_key_env;
    std::string reasoning_effort;
    std::string verbosity;
    std::string prompt;
    bool fail_on_error;
};

```
