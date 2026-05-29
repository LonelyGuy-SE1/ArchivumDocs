# ~GitRuntime

- Type: method
- Source: `src/main.cpp:41-41`
- Interface hash: `9127019589102076646`
- Source hash: `14650765020867691412`

## Signature

```cpp
~GitRuntime()
```

## References

- `GitRuntime`
- `archivum`
- `shutdown_git_subsystem`

## Source

```cpp
    ~GitRuntime() { archivum::shutdown_git_subsystem(); }

```
