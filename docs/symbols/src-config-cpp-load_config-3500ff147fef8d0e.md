# load_config

- Type: function
- Source: `src/config.cpp:132-159`
- Interface hash: `10955362048418545893`
- Source hash: `8466705522732113774`

## Signature

```cpp
ArchivumConfig load_config(const std::filesystem::path& path)
```

## References

- `ArchivumConfig`
- `apply_env`
- `apply_pair`
- `cleaned`
- `config`
- `delimiter`
- `empty`
- `equals`
- `file`
- `filesystem`
- `find`
- `front`
- `getline`
- `ifstream`
- `is_open`
- `line`
- `npos`
- `path`
- `std`
- `string`
- `substr`
- `trim`

## Source

```cpp
ArchivumConfig load_config(const std::filesystem::path& path) {
    ArchivumConfig config;
    std::ifstream file(path);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::string cleaned = trim(line);
            if (cleaned.empty() || cleaned.front() == '#') {
                continue;
            }

            size_t delimiter = cleaned.find(':');
            size_t equals = cleaned.find('=');
            if (delimiter == std::string::npos || (equals != std::string::npos && equals < delimiter)) {
                delimiter = equals;
            }
            if (delimiter == std::string::npos) {
                continue;
            }

            apply_pair(config, cleaned.substr(0, delimiter), cleaned.substr(delimiter + 1));
        }
    }

    apply_env(config);
    return config;
}

```
