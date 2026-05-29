# discover_source_files

- Type: function
- Source: `src/main.cpp:61-84`
- Interface hash: `2861702976012398251`
- Source hash: `15219791973799760794`

## Signature

```cpp
std::vector<std::string> discover_source_files(const std::filesystem::path& root)
```

## References

- `begin`
- `directory_entry`
- `directory_options`
- `disable_recursion_pending`
- `end`
- `entry`
- `files`
- `filesystem`
- `generic_string`
- `ignored_directory`
- `is_directory`
- `is_regular_file`
- `is_source_file`
- `it`
- `path`
- `push_back`
- `recursive_directory_iterator`
- `relative`
- `root`
- `skip_permission_denied`
- `sort`
- `std`
- `string`
- `vector`

## Source

```cpp
std::vector<std::string> discover_source_files(const std::filesystem::path& root) {
    std::vector<std::string> files;
    std::filesystem::recursive_directory_iterator it(root, std::filesystem::directory_options::skip_permission_denied);
    std::filesystem::recursive_directory_iterator end;

    for (; it != end; ++it) {
        const std::filesystem::directory_entry& entry = *it;
        if (entry.is_directory()) {
            if (ignored_directory(entry.path())) {
                it.disable_recursion_pending();
            }
            continue;
        }

        if (!entry.is_regular_file() || !is_source_file(entry.path())) {
            continue;
        }

        files.push_back(std::filesystem::relative(entry.path(), root).generic_string());
    }

    std::sort(files.begin(), files.end());
    return files;
}

```
