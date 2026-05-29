# git_scanner_reports_changed_ranges

- Type: function
- Source: `tests/test_main.cpp:146-186`
- Interface hash: `15277422122843127551`
- Source hash: `1540069234767000588`

## Signature

```cpp
void git_scanner_reports_changed_ranges()
```

## References

- `FileDiff`
- `GitScanner`
- `ModifiedRange`
- `ScopedTemp`
- `archivum`
- `base`
- `calculate_diff`
- `diff`
- `diffs`
- `empty`
- `end_line`
- `file_path`
- `first_scan`
- `has_value`
- `head`
- `modified_lines`
- `optional`
- `parent`
- `parent_of`
- `path`
- `quote`
- `range`
- `require`
- `resolve_reference`
- `run_command`
- `saw_header`
- `saw_source`
- `scanner`
- `size`
- `start_line`
- `std`
- `string`
- `temp`
- `value`
- `vector`
- `write_file`

## Source

```cpp
void git_scanner_reports_changed_ranges() {
    ScopedTemp temp;
    run_command("git init -q " + quote(temp.path()));
    run_command("git -C " + quote(temp.path()) + " config user.email archivum@example.invalid");
    run_command("git -C " + quote(temp.path()) + " config user.name Archivum");
    run_command("git -C " + quote(temp.path()) + " config commit.gpgsign false");
    run_command("git -C " + quote(temp.path()) + " config core.autocrlf false");

    write_file(temp.path() / "src" / "tool.cpp", "int answer() {\n    return 41;\n}\n");
    run_command("git -C " + quote(temp.path()) + " add .");
    run_command("git -C " + quote(temp.path()) + " commit -q -m initial");

    archivum::GitScanner first_scan(temp.path().string());
    std::string base = first_scan.resolve_reference("HEAD");

    write_file(temp.path() / "src" / "tool.cpp", "int answer() {\n    return 42;\n}\n");
    write_file(temp.path() / "include" / "tool.hpp", "int answer();\n");
    run_command("git -C " + quote(temp.path()) + " add .");
    run_command("git -C " + quote(temp.path()) + " commit -q -m update");

    archivum::GitScanner scanner(temp.path().string());
    std::string head = scanner.resolve_reference("HEAD");
    std::optional<std::string> parent = scanner.parent_of(head);
    std::vector<archivum::FileDiff> diffs = scanner.calculate_diff(base, head);

    require(parent.has_value() && parent.value() == base, "parent commit resolution failed");
    require(diffs.size() == 2, "diff did not include modified and added files");

    bool saw_source = false;
    bool saw_header = false;
    for (const archivum::FileDiff& diff : diffs) {
        saw_source = saw_source || diff.file_path == "src/tool.cpp";
        saw_header = saw_header || diff.file_path == "include/tool.hpp";
        require(!diff.modified_lines.empty(), "diff returned empty ranges");
        for (const archivum::ModifiedRange& range : diff.modified_lines) {
            require(range.start_line >= 1 && range.end_line >= range.start_line, "invalid changed range");
        }
    }

    require(saw_source && saw_header, "expected diff paths missing");
}

```
