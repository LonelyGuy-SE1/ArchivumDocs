# oid_to_string

- Type: function
- Source: `src/git_utils.cpp:21-25`
- Interface hash: `13011748508539796726`
- Source hash: `12171902419908235479`

## Signature

```cpp
std::string oid_to_string(const git_oid* oid)
```

## References

- `GIT_OID_HEXSZ`
- `git_oid`
- `git_oid_tostr`
- `oid`
- `std`
- `string`
- `value`

## Source

```cpp
std::string oid_to_string(const git_oid* oid) {
    char value[GIT_OID_HEXSZ + 1];
    git_oid_tostr(value, sizeof(value), oid);
    return value;
}

```
