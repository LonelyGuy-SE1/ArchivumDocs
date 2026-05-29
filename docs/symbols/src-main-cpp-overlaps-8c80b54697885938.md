# overlaps

- Type: function
- Source: `src/main.cpp:159-166`
- Interface hash: `11352494097033052614`
- Source hash: `5984040378261772572`

## Signature

```cpp
bool overlaps(const archivum::Node& node, const std::vector<archivum::ModifiedRange>& ranges)
```

## References

- `ModifiedRange`
- `Node`
- `archivum`
- `end_line`
- `node`
- `range`
- `ranges`
- `start_line`
- `std`
- `vector`

## Source

```cpp
bool overlaps(const archivum::Node& node, const std::vector<archivum::ModifiedRange>& ranges) {
    for (const archivum::ModifiedRange& range : ranges) {
        if (range.start_line <= node.end_line && range.end_line >= node.start_line) {
            return true;
        }
    }
    return false;
}

```
