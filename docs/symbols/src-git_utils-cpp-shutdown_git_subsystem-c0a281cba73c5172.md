# shutdown_git_subsystem

- Type: function
- Source: `src/git_utils.cpp:31-31`
- Interface hash: `8589466256352475083`
- Source hash: `9955199390086989334`

## Signature

```cpp
void shutdown_git_subsystem()
```

## References

- `git_libgit2_shutdown`

## Source

```cpp
void shutdown_git_subsystem() { git_libgit2_shutdown(); }

```
