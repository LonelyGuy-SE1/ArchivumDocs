# GitScanner::GitScanner

- Type: function
- Source: `src/git_utils.cpp:33-39`
- Interface hash: `13882695378004821769`
- Source hash: `6559196994553911477`

## Signature

```cpp
GitScanner::GitScanner(const std::string& repo_path)
```

## References

- `c_str`
- `error`
- `git_repository_open`
- `last_git_error`
- `repo`
- `repo_path`
- `runtime_error`
- `std`
- `string`

## Source

```cpp
GitScanner::GitScanner(const std::string& repo_path) {
    repo = nullptr;
    int error = git_repository_open(&repo, repo_path.c_str());
    if (error < 0) {
        throw std::runtime_error(last_git_error("Failed to open Git repository at " + repo_path));
    }
}

```
