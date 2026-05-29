# CliOptions

- Type: struct
- Source: `src/main.cpp:33-36`
- Interface hash: `9830707902177973788`
- Source hash: `12429975952340512562`

## Signature

```cpp
struct CliOptions
```

## References

- `config_path`
- `coordinates`
- `filesystem`
- `path`
- `std`
- `string`
- `vector`

## Source

```cpp
struct CliOptions {
    std::filesystem::path config_path = ".archivum.yml";
    std::vector<std::string> coordinates;
};

```
