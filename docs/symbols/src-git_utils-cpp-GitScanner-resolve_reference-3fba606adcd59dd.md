# GitScanner::resolve_reference

- Type: function
- Source: `src/git_utils.cpp:146-162`
- Interface hash: `7173447561615409197`
- Source hash: `1710548601534487514`

## Signature

```cpp
std::string GitScanner::resolve_reference(const std::string& reference) const
```

## References

- `GitScanner`
- `c_str`
- `error`
- `git_object`
- `git_object_free`
- `git_object_id`
- `git_oid`
- `git_revparse_single`
- `last_git_error`
- `object`
- `oid`
- `oid_to_string`
- `reference`
- `repo`
- `runtime_error`
- `std`
- `string`
- `value`

## Source

```cpp
std::string GitScanner::resolve_reference(const std::string& reference) const {
    git_object* object = nullptr;
    int error = git_revparse_single(&object, repo, reference.c_str());
    if (error < 0 || object == nullptr) {
        throw std::runtime_error(last_git_error("Failed to resolve Git reference " + reference));
    }

    const git_oid* oid = git_object_id(object);
    if (oid == nullptr) {
        git_object_free(object);
        throw std::runtime_error("[ArchivumDocs] FATAL: Git reference does not resolve to an object: " + reference);
    }

    std::string value = oid_to_string(oid);
    git_object_free(object);
    return value;
}

```
