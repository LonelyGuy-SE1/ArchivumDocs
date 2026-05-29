# stable_hash

- Type: function
- Source: `src/parser.cpp:20-27`
- Interface hash: `245245264662726832`
- Source hash: `1264302689977912347`

## Signature

```cpp
uint64_t stable_hash(std::string_view value)
```

## References

- `c`
- `hash`
- `std`
- `string_view`
- `value`

## Source

```cpp
uint64_t stable_hash(std::string_view value) {
    uint64_t hash = 14695981039346656037ull;
    for (unsigned char c : value) {
        hash ^= c;
        hash *= 1099511628211ull;
    }
    return hash;
}

```
