# provider_request

- Type: function
- Source: `src/main.cpp:228-239`
- Interface hash: `8706382991494355474`
- Source hash: `10119976531838918970`

## Signature

```cpp
archivum::ProviderRequest provider_request(const archivum::ArchivumConfig& config, const std::string& prompt)
```

## References

- `ArchivumConfig`
- `ProviderRequest`
- `api_key_env`
- `archivum`
- `config`
- `endpoint`
- `fail_on_error`
- `fail_on_provider_error`
- `model`
- `prompt`
- `provider`
- `reasoning_effort`
- `request`
- `std`
- `string`
- `verbosity`

## Source

```cpp
archivum::ProviderRequest provider_request(const archivum::ArchivumConfig& config, const std::string& prompt) {
    archivum::ProviderRequest request;
    request.provider = config.provider == "auto" ? "openai" : config.provider;
    request.endpoint = config.endpoint;
    request.model = config.model;
    request.api_key_env = config.api_key_env;
    request.reasoning_effort = config.reasoning_effort;
    request.verbosity = config.verbosity;
    request.prompt = prompt;
    request.fail_on_error = config.fail_on_provider_error;
    return request;
}

```
