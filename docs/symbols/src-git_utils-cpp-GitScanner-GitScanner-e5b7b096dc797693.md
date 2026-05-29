# GitScanner::~GitScanner

- Type: function
- Source: `src/git_utils.cpp:41-45`
- Interface hash: `1974449236653926330`
- Source hash: `6287078469978766943`

## Signature

```cpp
GitScanner::~GitScanner()
```

## References

- `GitScanner`
- `git_repository_free`
- `repo`

## Source

```cpp
GitScanner::~GitScanner() {
    if (repo != nullptr) {
        git_repository_free(repo);
    }
}

```
