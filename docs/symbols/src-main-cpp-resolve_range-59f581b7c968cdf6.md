# resolve_range

- Type: function
- Source: `src/main.cpp:132-157`
- Interface hash: `18408041468797764122`
- Source hash: `16629017203115593798`

## Signature

```cpp
ExecutionRange resolve_range(const std::vector<std::string>& coordinates, archivum::GitScanner& scanner)
```

## References

- `ExecutionRange`
- `GitScanner`
- `archivum`
- `arg`
- `base`
- `coordinates`
- `empty`
- `has_value`
- `head`
- `initial`
- `kInitialCommit`
- `optional`
- `parent`
- `parent_of`
- `resolve_reference`
- `runtime_error`
- `scanner`
- `size`
- `std`
- `string`
- `value_or`
- `vector`

## Source

```cpp
ExecutionRange resolve_range(const std::vector<std::string>& coordinates, archivum::GitScanner& scanner) {
    if (coordinates.empty()) {
        std::string head = scanner.resolve_reference("HEAD");
        std::optional<std::string> parent = scanner.parent_of(head);
        return {parent.value_or(kInitialCommit), head, parent.has_value() ? "auto" : "initial", !parent.has_value()};
    }

    if (coordinates.size() == 1) {
        std::string arg = coordinates[0];
        std::string head = scanner.resolve_reference(arg);
        std::optional<std::string> parent = scanner.parent_of(head);
        return {parent.value_or(kInitialCommit), head, "head-ref", !parent.has_value()};
    }

    if (coordinates.size() == 2) {
        std::string base = coordinates[0];
        std::string head = scanner.resolve_reference(coordinates[1]);
        bool initial = base == kInitialCommit;
        if (!initial) {
            base = scanner.resolve_reference(base);
        }
        return {base, head, "explicit", initial};
    }

    throw std::runtime_error("[ArchivumDocs] FATAL: Expected zero, one, or two Git coordinates.");
}

```
