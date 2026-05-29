# GitScanner::parent_of

- Type: function
- Source: `src/git_utils.cpp:164-179`
- Interface hash: `12869496262196345073`
- Source hash: `16631371167149080607`

## Signature

```cpp
std::optional<std::string> GitScanner::parent_of(const std::string& sha) const
```

## References

- `CommitPtr`
- `GitScanner`
- `commit`
- `get`
- `git_commit`
- `git_commit_free`
- `git_commit_id`
- `git_commit_parent`
- `git_commit_parentcount`
- `last_git_error`
- `lookup_commit`
- `nullopt`
- `oid_to_string`
- `optional`
- `parent`
- `parent_guard`
- `runtime_error`
- `sha`
- `std`
- `string`
- `unique_ptr`

## Source

```cpp
std::optional<std::string> GitScanner::parent_of(const std::string& sha) const {
    using CommitPtr = std::unique_ptr<git_commit, decltype(&git_commit_free)>;
    CommitPtr commit(lookup_commit(sha), git_commit_free);

    if (git_commit_parentcount(commit.get()) == 0) {
        return std::nullopt;
    }

    git_commit* parent = nullptr;
    if (git_commit_parent(&parent, commit.get(), 0) < 0) {
        throw std::runtime_error(last_git_error("Failed to resolve parent commit for " + sha));
    }
    CommitPtr parent_guard(parent, git_commit_free);

    return oid_to_string(git_commit_id(parent_guard.get()));
}

```
