# main

- Type: function
- Source: `src/main.cpp:338-345`
- Interface hash: `4510716788629620878`
- Source hash: `7292126862172104891`

## Signature

```cpp
int main(int argc, char* argv[])
```

## References

- `argc`
- `argv`
- `cerr`
- `error`
- `exception`
- `run`
- `std`
- `what`

## Source

```cpp
int main(int argc, char* argv[]) {
    try {
        return run(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << error.what() << "\n";
        return 1;
    }
}

```
