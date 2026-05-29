---
layout: default
title: "Symbol: render_index"
---

# render_index

| Metadata | Value |
| :--- | :--- |
| **Type** | Function |
| **Location** | `src/docs.cpp:148-205` |
| **Interface Hash** | `17290089167313208972` |
| **Source Hash** | `17415896943797412338` |

## Signature

```cpp
std::string render_index(const ArchivumConfig& config, const AnalysisReport& report, const std::string& generated_update, const std::filesystem::path& docs_root)
```

## Dependencies

- `AnalysisReport`
- `ArchivumConfig`
- `Node`
- `base_sha`
- `changed_file_count`
- `config`
- `context_nodes`
- `docs_root`
- `empty`
- `file_path`
- `filesystem`
- `generated_update`
- `graph_edge_count`
- `graph_node_count`
- `head_sha`
- `index_file`
- `mutated_nodes`
- `name`
- `node`
- `ostringstream`
- `out`
- `path`
- `relative_link`
- `report`
- `short_sha`
- `size`
- `source_file_count`
- `start_line`
- `std`
- `str`
- `string`
- `symbol_filename`
- `symbol_path`
- `symbols_dir`
- `type`
- `type_name`

## Implementation

```cpp
std::string render_index(const ArchivumConfig& config, const AnalysisReport& report,
                         const std::string& generated_update, const std::filesystem::path& docs_root) {
    std::ostringstream out;
    out << "---\n";
    out << "layout: default\n";
    out << "title: \"ArchivumDocs | Code Intelligence\"\n";
    out << "---\n\n";

    out << "# ArchivumDocs\n\n";

    out << "## System Status\n\n";
    out << "| Statistic | Value |\n";
    out << "| :--- | :--- |\n";
    out << "| **Analysis Range** | `" << short_sha(report.base_sha) << "` &rarr; `" << short_sha(report.head_sha)
        << "` |\n";
    out << "| **Source Files** | " << report.source_file_count << " |\n";
    out << "| **Changed Files** | " << report.changed_file_count << " |\n";
    out << "| **Graph Density** | " << report.graph_node_count << " symbols, " << report.graph_edge_count
        << " edges |\n";
    out << "| **Impact Radius** | " << report.mutated_nodes.size() << " mutated, " << report.context_nodes.size()
        << " context |\n\n";

    if (!generated_update.empty()) {
        out << "## AI Analysis\n\n";
        out << "> " << generated_update << "\n\n";
    }

    out << "## Changes & Impact\n\n";
    if (report.mutated_nodes.empty()) {
        out << "*No structural changes detected in this range.*\n";
    } else {
        out << "| Symbol | Type | Location |\n";
        out << "| :--- | :--- | :--- |\n";
        for (const Node& node : report.mutated_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "| [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") | ";
            out << type_name(node.type) << " | ";
            out << "`" << node.file_path << ":" << node.start_line << "` |\n";
        }
        out << "\n";
    }

    out << "## Downstream Context\n\n";
    if (report.context_nodes.empty()) {
        out << "*No downstream impacts identified.*\n";
    } else {
        out << "| Symbol | Type | Relationship |\n";
        out << "| :--- | :--- | :--- |\n";
        for (const Node& node : report.context_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "| [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") | ";
            out << type_name(node.type) << " | ";
            out << "Contextual dependency |\n";
        }
    }

    return out.str();
}

```
