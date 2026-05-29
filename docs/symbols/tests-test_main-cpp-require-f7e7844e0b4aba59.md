# require

- Type: function
- Source: `tests/test_main.cpp:45-49`
- Interface hash: `3731120416469675852`
- Source hash: `3565060211577432779`

## Signature

```cpp
void require(bool condition, const std::string& message)
```

## References

- `condition`
- `message`
- `runtime_error`
- `std`
- `string`

## Source

```cpp
void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

```
