# run_command

- Type: function
- Source: `tests/test_main.cpp:60-63`
- Interface hash: `5972373344981117565`
- Source hash: `2970523372681653727`

## Signature

```cpp
void run_command(const std::string& command)
```

## References

- `c_str`
- `code`
- `command`
- `require`
- `std`
- `string`
- `system`

## Source

```cpp
void run_command(const std::string& command) {
    int code = std::system(command.c_str());
    require(code == 0, "command failed: " + command);
}

```
