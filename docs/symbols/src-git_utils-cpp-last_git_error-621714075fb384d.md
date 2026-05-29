# last_git_error

- Type: function
- Source: `src/git_utils.cpp:13-19`
- Interface hash: `12728385349285301783`
- Source hash: `1016883202768862931`

## Signature

```cpp
std::string last_git_error(const std::string& action)
```

## References

- `action`
- `error`
- `git_error`
- `git_error_last`
- `message`
- `std`
- `string`

## Source

```cpp
std::string last_git_error(const std::string& action) {
    const git_error* error = git_error_last();
    if (error != nullptr && error->message != nullptr) {
        return "[ArchivumDocs] FATAL: " + action + ": " + error->message;
    }
    return "[ArchivumDocs] FATAL: " + action + ".";
}

```
