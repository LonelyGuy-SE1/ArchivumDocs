---
layout: default
title: "Symbol: write_documentation"
---

# write_documentation

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:265-297` |
| **Interface Hash** | `9884305512364725519` |
| **Source Hash** | `8348223628830197773` |

## Signature

```cpp
DocumentationWriteResult write_documentation(const ArchivumConfig& config, const AnalysisReport& report, const std::string& generated_update, const std::filesystem::path& root)
```

## Dependencies

- `AnalysisReport`
- `ArchivumConfig`
- `DocumentationWriteResult`
- `Node`
- `NodeId`
- `changed`
- `config`
- `context_nodes`
- `docs_dir`
- `docs_root`
- `filesystem`
- `generated_update`
- `id`
- `index_file`
- `index_path`
- `insert`
- `manifest_file`
- `manifest_path`
- `node`
- `path`
- `push_back`
- `read_slice`
- `render_index`
- `render_manifest`
- `render_symbol_page`
- `report`
- `result`
- `root`
- `second`
- `seen`
- `source`
- `std`
- `string`
- `symbol_filename`
- `symbols_dir`
- `symbols_root`
- `unordered_set`
- `write_if_changed`
- `written_files`

## Implementation

```cpp
DocumentationWriteResult write_documentation(const ArchivumConfig& config, const AnalysisReport& report,
                                             const std::string& generated_update, const std::filesystem::path& root) {
    DocumentationWriteResult result;
    std::filesystem::path docs_root = root / config.docs_dir;
    std::filesystem::path symbols_root = docs_root / config.symbols_dir;

    std::unordered_set<NodeId> seen;
    for (const Node& node : report.context_nodes) {
        if (!seen.insert(node.id).second) {
            continue;
        }
        std::filesystem::path path = symbols_root / symbol_filename(node);
        std::string source = read_slice(root, node, 24000);
        if (write_if_changed(path, render_symbol_page(node, source))) {
            result.changed = true;
            result.written_files.push_back(path);
        }
    }

    std::filesystem::path index_path = docs_root / config.index_file;
    if (write_if_changed(index_path, render_index(config, report, generated_update, docs_root))) {
        result.changed = true;
        result.written_files.push_back(index_path);
    }

    std::filesystem::path manifest_path = docs_root / config.manifest_file;
    if (write_if_changed(manifest_path, render_manifest(report))) {
        result.changed = true;
        result.written_files.push_back(manifest_path);
    }

    return result;
}

```
