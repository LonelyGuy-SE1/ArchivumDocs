---
layout: default
title: "Symbol: main"
---

# main

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/main.cpp:337-344` |
| **Interface Hash** | `4510716788629620878` |
| **Source Hash** | `7292126862172104891` |

## Signature

```cpp
int main(int argc, char* argv[])
```

## Dependencies

- `argc`
- `argv`
- `cerr`
- `error`
- `exception`
- `run`
- `std`
- `what`

## Implementation

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
