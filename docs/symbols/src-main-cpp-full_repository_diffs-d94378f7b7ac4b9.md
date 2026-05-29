# full_repository_diffs

- Type: function
- Source: `src/main.cpp:86-98`
- Interface hash: `2066604460308760143`
- Source hash: `2322409314278086640`

## Signature

```cpp
std::vector<archivum::FileDiff> full_repository_diffs(const std::vector<std::string>& files)
```

## References

- `FileDiff`
- `archivum`
- `diff`
- `diffs`
- `file`
- `file_path`
- `files`
- `max`
- `modified_lines`
- `move`
- `numeric_limits`
- `push_back`
- `reserve`
- `size`
- `std`
- `string`
- `vector`

## Source

```cpp
std::vector<archivum::FileDiff> full_repository_diffs(const std::vector<std::string>& files) {
    std::vector<archivum::FileDiff> diffs;
    diffs.reserve(files.size());

    for (const std::string& file : files) {
        archivum::FileDiff diff;
        diff.file_path = file;
        diff.modified_lines.push_back({1, std::numeric_limits<uint32_t>::max()});
        diffs.push_back(std::move(diff));
    }

    return diffs;
}

```
