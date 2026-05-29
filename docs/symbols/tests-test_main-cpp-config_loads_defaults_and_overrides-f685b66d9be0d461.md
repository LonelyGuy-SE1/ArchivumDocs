# config_loads_defaults_and_overrides

- Type: function
- Source: `tests/test_main.cpp:188-204`
- Interface hash: `16945464477121461827`
- Source hash: `15178330440220169626`

## Signature

```cpp
void config_loads_defaults_and_overrides()
```

## References

- `ArchivumConfig`
- `ScopedTemp`
- `archivum`
- `config`
- `config_path`
- `docs_dir`
- `fail_on_provider_error`
- `filesystem`
- `load_config`
- `max_symbols`
- `model`
- `path`
- `require`
- `std`
- `temp`
- `write_file`
- `write_mode`
- `write_mode_name`

## Source

```cpp
void config_loads_defaults_and_overrides() {
    ScopedTemp temp;
    std::filesystem::path config_path = temp.path() / ".archivum.yml";
    write_file(config_path,
               "docs_dir: reference\n"
               "model: gpt-test\n"
               "write_mode: none\n"
               "max_symbols: 7\n"
               "fail_on_provider_error: true\n");

    archivum::ArchivumConfig config = archivum::load_config(config_path);
    require(config.docs_dir == "reference", "docs_dir override failed");
    require(config.model == "gpt-test", "model override failed");
    require(archivum::write_mode_name(config.write_mode) == "none", "write mode override failed");
    require(config.max_symbols == 7, "max_symbols override failed");
    require(config.fail_on_provider_error, "provider error override failed");
}

```
