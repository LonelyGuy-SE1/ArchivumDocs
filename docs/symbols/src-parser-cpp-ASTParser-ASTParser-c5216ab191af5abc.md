# ASTParser::~ASTParser

- Type: function
- Source: `src/parser.cpp:190-194`
- Interface hash: `6573548537475521654`
- Source hash: `353064896328187822`

## Signature

```cpp
ASTParser::~ASTParser()
```

## References

- `ASTParser`
- `parser`
- `ts_parser_delete`

## Source

```cpp
ASTParser::~ASTParser() {
    if (parser != nullptr) {
        ts_parser_delete(parser);
    }
}

```
