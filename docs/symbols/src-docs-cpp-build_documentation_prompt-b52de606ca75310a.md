# build_documentation_prompt

- Type: function
- Source: `src/docs.cpp:199-238`
- Interface hash: `12295667639138194231`
- Source hash: `6680592853663772670`

## Signature

```cpp
std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report, const std::filesystem::path& root)
```

## References

- `AnalysisReport`
- `ArchivumConfig`
- `Node`
- `base_sha`
- `config`
- `consumed`
- `context_nodes`
- `emitted`
- `empty`
- `end_line`
- `file_path`
- `filesystem`
- `head_sha`
- `max_context_bytes`
- `max_symbols`
- `min`
- `mutated_nodes`
- `name`
- `node`
- `ostringstream`
- `path`
- `prompt`
- `read_slice`
- `remaining`
- `report`
- `root`
- `signature`
- `size`
- `source`
- `start_line`
- `std`
- `str`
- `string`
- `type`
- `type_name`

## Source

```cpp
std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report,
                                       const std::filesystem::path& root) {
    std::ostringstream prompt;
    prompt << "You are ArchivumDocs, a documentation maintainer. Write a concise Markdown update for the docs page. ";
    prompt << "Focus on changed public behavior, symbol responsibilities, and downstream documentation impact. ";
    prompt << "Do not include code fences unless they clarify an API signature.\n\n";
    prompt << "Range: " << report.base_sha << " -> " << report.head_sha << "\n";
    prompt << "Mutated symbols: " << report.mutated_nodes.size() << "\n";
    prompt << "Context symbols: " << report.context_nodes.size() << "\n\n";

    size_t remaining = config.max_context_bytes;
    size_t emitted = 0;
    for (const Node& node : report.context_nodes) {
        if (emitted >= config.max_symbols || remaining == 0) {
            break;
        }

        std::string source = read_slice(root, node, std::min<size_t>(remaining, 12000));
        size_t consumed = std::min(remaining, source.size());
        remaining -= consumed;
        ++emitted;

        prompt << "## " << node.name << "\n";
        prompt << "Type: " << type_name(node.type) << "\n";
        prompt << "Location: " << node.file_path << ":" << node.start_line << "-" << node.end_line << "\n";
        if (!node.signature.empty()) {
            prompt << "Signature: " << node.signature << "\n";
        }
        if (!source.empty()) {
            prompt << "Source:\n" << source << "\n";
        }
        prompt << "\n";
    }

    if (report.context_nodes.size() > emitted) {
        prompt << "Context truncated after " << emitted << " symbols.\n";
    }

    return prompt.str();
}

```
