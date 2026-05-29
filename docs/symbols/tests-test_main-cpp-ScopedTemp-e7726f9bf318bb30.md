# ScopedTemp

- Type: method
- Source: `tests/test_main.cpp:26-30`
- Interface hash: `13135239578601645444`
- Source hash: `16981781204046172099`

## Signature

```cpp
ScopedTemp()
```

## References

- `chrono`
- `count`
- `create_directories`
- `filesystem`
- `now`
- `path_`
- `stamp`
- `std`
- `steady_clock`
- `temp_directory_path`
- `time_since_epoch`
- `to_string`

## Source

```cpp
    ScopedTemp() {
        auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() / ("archivum_tests_" + std::to_string(stamp));
        std::filesystem::create_directories(path_);
    }

```
