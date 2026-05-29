# provider_name

- Type: function
- Source: `src/api.cpp:18-29`
- Interface hash: `15812512912004546997`
- Source hash: `6753613693766508300`

## Signature

```cpp
std::string provider_name()
```

## References

- `getenv`
- `present`
- `std`
- `string`

## Source

```cpp
std::string provider_name() {
    if (present(std::getenv("ARCHIVUM_LLM_ENDPOINT"))) {
        return "custom";
    }
    if (present(std::getenv("OPENAI_API_KEY"))) {
        return "openai";
    }
    if (present(std::getenv("ANTHROPIC_API_KEY"))) {
        return "anthropic";
    }
    return "shadow";
}

```
