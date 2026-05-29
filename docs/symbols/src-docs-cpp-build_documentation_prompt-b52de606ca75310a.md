---
layout: default
title: "Symbol: build_documentation_prompt"
---

# build_documentation_prompt

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:224-263` |
| **Interface Hash** | `12295667639138194231` |
| **Source Hash** | `3715835764050079250` |

## Signature

```cpp
std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report, const std::filesystem::path& root)
```

## Dependencies

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

## Implementation

```cpp
std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report,
                                       const std::filesystem::path& root) {
    std::ostringstream prompt;
    prompt << "You are ArchivumDocs, a documentation maintainer for an elegant and high-integrity C++ codebase. ";
    prompt << "Write a sophisticated and professional Markdown summary of the changes in this range. ";
    prompt << "Focus on architectural implications, changes in API contracts, and how downstream components might be affected. ";
    prompt << "Maintain a refined, technical tone. Do not use code fences for the summary text itself.\n\n";
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

        prompt << "### " << node.name << " (" << type_name(node.type) << ")\n";
        prompt << "Location: " << node.file_path << ":" << node.start_line << "-" << node.end_line << "\n";
        if (!node.signature.empty()) {
            prompt << "Signature: " << node.signature << "\n";
        }
        if (!source.empty()) {
            prompt << "Source Snippet:\n" << source << "\n";
        }
        prompt << "\n";
    }

    if (report.context_nodes.size() > emitted) {
        prompt << "Note: Context truncated after " << emitted << " symbols for efficiency.\n";
    }

    return prompt.str();
}

```
