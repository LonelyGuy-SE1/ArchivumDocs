# present

- Type: function
- Source: `src/api.cpp:16-16`
- Interface hash: `6147354491670422613`
- Source hash: `15313797413150996835`

## Signature

```cpp
bool present(const char* value)
```

## References

- `value`

## Source

```cpp
bool present(const char* value) { return value != nullptr && value[0] != '\0'; }

```
