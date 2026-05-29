# GitScanner::lookup_commit

- Type: function
- Source: `src/git_utils.cpp:47-61`
- Interface hash: `15017511586612666878`
- Source hash: `17556663980117902165`

## Signature

```cpp
git_commit* GitScanner::lookup_commit(const std::string& sha) const
```

## References

- `GitScanner`
- `c_str`
- `commit`
- `error`
- `git_commit`
- `git_commit_lookup`
- `git_oid`
- `git_oid_fromstr`
- `last_git_error`
- `oid`
- `repo`
- `runtime_error`
- `sha`
- `std`
- `string`

## Source

```cpp
git_commit* GitScanner::lookup_commit(const std::string& sha) const {
    git_oid oid;
    int error = git_oid_fromstr(&oid, sha.c_str());
    if (error < 0) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Invalid SHA format: " + sha);
    }

    git_commit* commit = nullptr;
    error = git_commit_lookup(&commit, repo, &oid);
    if (error < 0) {
        throw std::runtime_error(last_git_error("Commit SHA not found in storage: " + sha));
    }

    return commit;
}

```
