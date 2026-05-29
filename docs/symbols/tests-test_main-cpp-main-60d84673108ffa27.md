# main

- Type: function
- Source: `tests/test_main.cpp:275-295`
- Interface hash: `5561091575973751992`
- Source hash: `390801323986888620`

## Signature

```cpp
int main()
```

## References

- `archivum`
- `cerr`
- `config_loads_defaults_and_overrides`
- `documentation_writer_creates_index_symbols_and_manifest`
- `error`
- `exception`
- `git_scanner_reports_changed_ranges`
- `graph_handles_cycles`
- `graph_infers_transitive_impact`
- `init_git_subsystem`
- `parser_extracts_structural_metadata`
- `provider_skips_when_key_is_absent`
- `run_test`
- `shutdown_git_subsystem`
- `std`
- `what`

## Source

```cpp
int main() {
    archivum::init_git_subsystem();

    try {
        run_test("parser_extracts_structural_metadata", parser_extracts_structural_metadata);
        run_test("graph_infers_transitive_impact", graph_infers_transitive_impact);
        run_test("graph_handles_cycles", graph_handles_cycles);
        run_test("git_scanner_reports_changed_ranges", git_scanner_reports_changed_ranges);
        run_test("config_loads_defaults_and_overrides", config_loads_defaults_and_overrides);
        run_test("documentation_writer_creates_index_symbols_and_manifest",
                 documentation_writer_creates_index_symbols_and_manifest);
        run_test("provider_skips_when_key_is_absent", provider_skips_when_key_is_absent);
    } catch (const std::exception& error) {
        archivum::shutdown_git_subsystem();
        std::cerr << "[FAIL] " << error.what() << "\n";
        return 1;
    }

    archivum::shutdown_git_subsystem();
    return 0;
}

```
