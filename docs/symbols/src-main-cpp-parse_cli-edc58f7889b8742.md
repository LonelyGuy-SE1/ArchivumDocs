# parse_cli

- Type: function
- Source: `src/main.cpp:100-130`
- Interface hash: `3872946081530822530`
- Source hash: `75924668153550507`

## Signature

```cpp
CliOptions parse_cli(int argc, char* argv[])
```

## References

- `CliOptions`
- `arg`
- `argc`
- `argv`
- `config_path`
- `coordinates`
- `cout`
- `exit`
- `index`
- `options`
- `push_back`
- `runtime_error`
- `size`
- `std`
- `string`

## Source

```cpp
CliOptions parse_cli(int argc, char* argv[]) {
    CliOptions options;

    for (int index = 1; index < argc; ++index) {
        std::string arg = argv[index];
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: archivum [--config path] [base_sha head_sha]\n";
            std::cout << "       archivum [--config path] [head_ref]\n";
            std::cout << "       archivum [--config path]\n";
            std::exit(0);
        }
        if (arg == "--version") {
            std::cout << "ArchivumDocs 1.0.0\n";
            std::exit(0);
        }
        if (arg == "--config") {
            if (index + 1 >= argc) {
                throw std::runtime_error("[ArchivumDocs] FATAL: --config requires a path.");
            }
            options.config_path = argv[++index];
            continue;
        }
        options.coordinates.push_back(arg);
    }

    if (options.coordinates.size() > 2) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Expected zero, one, or two Git coordinates.");
    }

    return options;
}

```
