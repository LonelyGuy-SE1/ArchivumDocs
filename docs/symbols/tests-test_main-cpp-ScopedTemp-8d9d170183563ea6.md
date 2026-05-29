# ~ScopedTemp

- Type: method
- Source: `tests/test_main.cpp:32-37`
- Interface hash: `14990219456870877200`
- Source hash: `7457700319649303215`

## Signature

```cpp
~ScopedTemp()
```

## References

- `ScopedTemp`
- `error_code`
- `filename`
- `filesystem`
- `ignored`
- `path_`
- `remove_all`
- `starts_with`
- `std`
- `string`

## Source

```cpp
    ~ScopedTemp() {
        std::error_code ignored;
        if (path_.filename().string().starts_with("archivum_tests_")) {
            std::filesystem::remove_all(path_, ignored);
        }
    }

```
