# ASTParser::ASTParser

- Type: function
- Source: `src/parser.cpp:183-188`
- Interface hash: `15055179494770780192`
- Source hash: `16885443326475823770`

## Signature

```cpp
ASTParser::ASTParser()
```

## References

- `parser`
- `runtime_error`
- `std`
- `tree_sitter_cpp`
- `ts_parser_new`
- `ts_parser_set_language`

## Source

```cpp
ASTParser::ASTParser() {
    parser = ts_parser_new();
    if (parser == nullptr || !ts_parser_set_language(parser, tree_sitter_cpp())) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Failed to initialize the C++ parser.");
    }
}

```
