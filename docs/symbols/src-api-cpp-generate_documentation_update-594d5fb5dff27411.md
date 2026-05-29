# generate_documentation_update

- Type: function
- Source: `src/api.cpp:235-280`
- Interface hash: `2882631668053171521`
- Source hash: `9107358507196238787`

## Signature

```cpp
std::optional<std::string> generate_documentation_update(const ProviderRequest& request)
```

## References

- `ProviderRequest`
- `api_key_env`
- `binary`
- `body`
- `body_path`
- `c_str`
- `config`
- `config_path`
- `endpoint`
- `extract_response_text`
- `fail_on_error`
- `filesystem`
- `generic_string`
- `getenv`
- `has_value`
- `ios`
- `key`
- `nullopt`
- `ofstream`
- `optional`
- `path`
- `present`
- `provider`
- `remove`
- `request`
- `request_payload`
- `response`
- `run_capture`
- `runtime_error`
- `shell_quote`
- `std`
- `string`
- `temp_file`
- `text`

## Source

```cpp
std::optional<std::string> generate_documentation_update(const ProviderRequest& request) {
    const char* key = std::getenv(request.api_key_env.c_str());
    if (!present(key) || request.provider == "shadow" || request.provider == "none") {
        return std::nullopt;
    }

    std::filesystem::path body_path = temp_file("request.json");
    std::filesystem::path config_path = temp_file("curl.cfg");

    try {
        {
            std::ofstream body(body_path, std::ios::binary);
            body << request_payload(request);
        }
        {
            std::ofstream config(config_path, std::ios::binary);
            config << "silent\n";
            config << "show-error\n";
            config << "fail-with-body\n";
            config << "request = POST\n";
            config << "url = \"" << request.endpoint << "\"\n";
            config << "header = \"Content-Type: application/json\"\n";
            config << "header = \"Authorization: Bearer " << key << "\"\n";
            config << "data-binary = \"@" << body_path.generic_string() << "\"\n";
        }

        std::string response = run_capture("curl --config " + shell_quote(config_path));
        std::filesystem::remove(body_path);
        std::filesystem::remove(config_path);
        std::optional<std::string> text = extract_response_text(response);
        if (text.has_value()) {
            return text;
        }
        if (request.fail_on_error) {
            throw std::runtime_error("[ArchivumDocs] FATAL: Provider response did not contain text output.");
        }
    } catch (...) {
        std::filesystem::remove(body_path);
        std::filesystem::remove(config_path);
        if (request.fail_on_error) {
            throw;
        }
    }

    return std::nullopt;
}

```
