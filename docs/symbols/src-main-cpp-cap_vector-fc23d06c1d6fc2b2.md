# cap_vector

- Type: function
- Source: `src/main.cpp:222-226`
- Interface hash: `10991254964017375799`
- Source hash: `13313238543632290113`

## Signature

```cpp
void cap_vector(std::vector<T>& values, size_t limit)
```

## References

- `T`
- `limit`
- `resize`
- `size`
- `std`
- `values`
- `vector`

## Source

```cpp
void cap_vector(std::vector<T>& values, size_t limit) {
    if (values.size() > limit) {
        values.resize(limit);
    }
}

```
