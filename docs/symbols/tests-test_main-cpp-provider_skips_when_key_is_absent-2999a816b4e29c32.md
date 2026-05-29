# provider_skips_when_key_is_absent

- Type: function
- Source: `tests/test_main.cpp:254-266`
- Interface hash: `5554277694160525600`
- Source hash: `15133320245856538841`

## Signature

```cpp
void provider_skips_when_key_is_absent()
```

## References

- `ProviderRequest`
- `api_key_env`
- `archivum`
- `endpoint`
- `fail_on_error`
- `generate_documentation_update`
- `has_value`
- `model`
- `prompt`
- `provider`
- `reasoning_effort`
- `request`
- `require`
- `verbosity`

## Source

```cpp
void provider_skips_when_key_is_absent() {
    archivum::ProviderRequest request;
    request.provider = "openai";
    request.endpoint = "https://api.openai.com/v1/responses";
    request.model = "gpt-test";
    request.api_key_env = "ARCHIVUM_TEST_MISSING_KEY";
    request.reasoning_effort = "low";
    request.verbosity = "low";
    request.prompt = "hello";
    request.fail_on_error = false;

    require(!archivum::generate_documentation_update(request).has_value(), "provider should skip without credentials");
}

```
