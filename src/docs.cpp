#include <algorithm>
#include <archivum/docs.hpp>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace archivum {

namespace {

std::string type_name(NodeType type) {
    switch (type) {
        case NodeType::FUNCTION:
            return "Function";
        case NodeType::CLASS:
            return "Class";
        case NodeType::STRUCT:
            return "Struct";
        case NodeType::METHOD:
            return "Method";
        default:
            return "Symbol";
    }
}

std::string type_name_sentence(NodeType type) {
    switch (type) {
        case NodeType::FUNCTION:
            return "function";
        case NodeType::CLASS:
            return "class";
        case NodeType::STRUCT:
            return "struct";
        case NodeType::METHOD:
            return "method";
        default:
            return "symbol";
    }
}

std::string sanitize(std::string value) {
    for (char& c : value) {
        bool keep = std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_';
        if (!keep) {
            c = '-';
        }
    }

    value.erase(
        std::unique(value.begin(), value.end(), [](char left, char right) { return left == '-' && right == '-'; }),
        value.end());

    while (!value.empty() && value.front() == '-') {
        value.erase(value.begin());
    }
    while (!value.empty() && value.back() == '-') {
        value.pop_back();
    }
    return value.empty() ? "symbol" : value;
}

std::string short_sha(const std::string& sha) { return sha.size() > 8 ? sha.substr(0, 8) : sha; }

std::string relative_link(const std::filesystem::path& from, const std::filesystem::path& to) {
    return std::filesystem::relative(to, from.parent_path()).generic_string();
}

std::string symbol_filename(const Node& node) {
    std::ostringstream out;
    out << sanitize(node.file_path) << "-" << sanitize(node.name) << "-" << std::hex << node.id << ".html";
    return out.str();
}

std::string read_slice(const std::filesystem::path& root, const Node& node, size_t limit) {
    std::ifstream file(root / node.file_path);
    if (!file.is_open()) {
        return {};
    }

    std::ostringstream out;
    std::string line;
    uint32_t line_number = 0;
    size_t written = 0;

    while (std::getline(file, line)) {
        ++line_number;
        if (line_number < node.start_line) {
            continue;
        }
        if (line_number > node.end_line || written >= limit) {
            break;
        }

        size_t remaining = limit - written;
        if (line.size() + 1 > remaining) {
            out << line.substr(0, remaining);
            written = limit;
        } else {
            out << line << "\n";
            written += line.size() + 1;
        }
    }

    return out.str();
}

bool write_if_changed(const std::filesystem::path& path, const std::string& content) {
    std::ifstream existing(path, std::ios::binary);
    if (existing.is_open()) {
        std::ostringstream buffer;
        buffer << existing.rdbuf();
        if (buffer.str() == content) {
            return false;
        }
    }

    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    out << content;
    return true;
}

std::string render_symbol_page(const Node& node, const std::string& source) {
    std::ostringstream out;
    out << "---\n";
    out << "title: \"" << node.name << " | API Reference\"\n";
    out << "---\n\n";

    out << "# " << node.name << "\n\n";
    out << "> " << type_name(node.type) << " defined in `" << node.file_path << "`\n\n";
    out << "Use this page to understand how `" << node.name << "` fits into the codebase, what it depends on, and\n";
    out << "where the implementation lives.\n\n";

    out << "## At A Glance\n\n";
    out << "| Attribute | Details |\n";
    out << "| :--- | :--- |\n";
    out << "| **Symbol Type** | " << type_name(node.type) << " |\n";
    out << "| **Source File** | `" << node.file_path << "` |\n";
    out << "| **Source Range** | Lines " << node.start_line << " to " << node.end_line << " |\n";
    out << "| **Interface Hash** | `" << node.interface_hash << "` |\n\n";

    if (!node.signature.empty()) {
        out << "## Signature\n\n";
        out << "Call signature as extracted from the AST.\n\n";
        out << "```cpp\n" << node.signature << "\n```\n\n";
    }

    out << "## Dependencies\n\n";
    if (node.references.empty()) {
        out << "No external symbols were detected in this definition.\n\n";
    } else {
        out << "Symbols this definition refers to.\n\n";
        for (const std::string& reference : node.references) {
            out << "- `" << reference << "`\n";
        }
        out << "\n";
    }

    if (!source.empty()) {
        out << "## Implementation\n\n";
        out << "Source excerpt for quick review.\n\n";
        out << "```cpp\n" << source << "\n```\n";
    } else {
        out << "## Implementation\n\n";
        out << "No source excerpt available for this symbol.\n\n";
    }

    out << "## Related Documentation\n\n";
    out << "- Return to the [project index](../index.md) for the full change overview.\n";

    return out.str();
}

std::string render_index(const ArchivumConfig& config, const AnalysisReport& report,
                         const std::string& generated_update, const std::filesystem::path& docs_root) {
    std::ostringstream out;
    out << "---\n";
    std::string title = config.project_name.empty() ? "ArchivumDocs" : config.project_name;
    out << "title: \"" << title << " | Code Intelligence\"\n";
    out << "---\n\n";

    out << "# " << title << "\n\n";
    if (!config.project_tagline.empty()) {
        out << "> " << config.project_tagline << "\n\n";
    }

    out << "## Overview\n\n";
    out << "" << title << " maps code changes to documentation updates by analyzing symbols, dependency edges, and affected\n";
    out << "call sites. The tables below highlight what changed in this range and which parts of the codebase are\n";
    out << "impacted, so you can focus your review quickly.\n\n";

    out << "## How To Read This Page\n\n";
    out << "- **Changes & Impact** lists symbols directly touched by the diff.\n";
    out << "- **Downstream Context** lists dependencies that may be affected even if they were not edited.\n";
    out << "- **System Status** summarizes repository scale and the analysis range.\n\n";

    out << "## Getting Oriented\n\n";
    std::unordered_set<NodeId> mutated_ids;
    mutated_ids.reserve(report.mutated_nodes.size());
    for (const Node& node : report.mutated_nodes) {
        mutated_ids.insert(node.id);
    }

    std::vector<Node> key_symbols;
    key_symbols.reserve(12);
    std::unordered_set<NodeId> seen;
    for (const Node& node : report.mutated_nodes) {
        if (key_symbols.size() >= 12) {
            break;
        }
        if (seen.insert(node.id).second) {
            key_symbols.push_back(node);
        }
    }
    for (const Node& node : report.context_nodes) {
        if (key_symbols.size() >= 12) {
            break;
        }
        if (seen.insert(node.id).second) {
            key_symbols.push_back(node);
        }
    }

    if (key_symbols.empty()) {
        out << "No symbols were captured for this range.\n\n";
    } else {
        out << "| Symbol | Type | Status | Location |\n";
        out << "| :--- | :--- | :--- | :--- |\n";
        for (const Node& node : key_symbols) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "| [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") | ";
            out << type_name(node.type) << " | ";
            out << (mutated_ids.find(node.id) != mutated_ids.end() ? "Changed" : "Downstream") << " | ";
            out << "`" << node.file_path << ":" << node.start_line << "` |\n";
        }
        out << "\n";
    }

    out << "## Repository Map\n\n";
    out << "Start here to understand the highest-activity files in this analysis window.\n\n";

    struct FileSummary {
        std::string path;
        size_t symbol_count = 0;
        size_t mutated_count = 0;
    };

    std::unordered_map<std::string, FileSummary> file_map;
    std::unordered_set<NodeId> counted;
    for (const Node& node : report.context_nodes) {
        FileSummary& entry = file_map[node.file_path];
        entry.path = node.file_path;
        if (counted.insert(node.id).second) {
            ++entry.symbol_count;
        }
    }
    for (const Node& node : report.mutated_nodes) {
        FileSummary& entry = file_map[node.file_path];
        entry.path = node.file_path;
        ++entry.mutated_count;
        if (counted.insert(node.id).second) {
            ++entry.symbol_count;
        }
    }

    if (file_map.empty()) {
        out << "No files were captured for this range.\n\n";
    } else {
        std::vector<FileSummary> files;
        files.reserve(file_map.size());
        for (const auto& [_, summary] : file_map) {
            files.push_back(summary);
        }
        std::sort(files.begin(), files.end(), [](const FileSummary& left, const FileSummary& right) {
            if (left.mutated_count != right.mutated_count) {
                return left.mutated_count > right.mutated_count;
            }
            if (left.symbol_count != right.symbol_count) {
                return left.symbol_count > right.symbol_count;
            }
            return left.path < right.path;
        });

        out << "| File | Symbols | Changed Symbols |\n";
        out << "| :--- | ---: | ---: |\n";
        size_t limit = std::min<size_t>(8, files.size());
        for (size_t i = 0; i < limit; ++i) {
            const FileSummary& entry = files[i];
            out << "| `" << entry.path << "` | " << entry.symbol_count << " | " << entry.mutated_count << " |\n";
        }
        out << "\n";
    }

    out << "## Module Guide\n\n";
    out << "High-level grouping by top-level directory.\n\n";

    struct ModuleSummary {
        std::string name;
        size_t symbol_count = 0;
        size_t mutated_count = 0;
    };

    std::unordered_map<std::string, ModuleSummary> module_map;
    std::unordered_set<NodeId> module_counted;

    auto module_name_for = [](const std::string& path) {
        size_t slash = path.find('/');
        if (slash == std::string::npos) {
            return std::string("root");
        }
        return path.substr(0, slash);
    };

    for (const Node& node : report.context_nodes) {
        std::string module = module_name_for(node.file_path);
        ModuleSummary& entry = module_map[module];
        entry.name = module;
        if (module_counted.insert(node.id).second) {
            ++entry.symbol_count;
        }
    }
    for (const Node& node : report.mutated_nodes) {
        std::string module = module_name_for(node.file_path);
        ModuleSummary& entry = module_map[module];
        entry.name = module;
        ++entry.mutated_count;
        if (module_counted.insert(node.id).second) {
            ++entry.symbol_count;
        }
    }

    if (module_map.empty()) {
        out << "No modules were captured for this range.\n\n";
    } else {
        std::vector<ModuleSummary> modules;
        modules.reserve(module_map.size());
        for (const auto& [_, summary] : module_map) {
            modules.push_back(summary);
        }
        std::sort(modules.begin(), modules.end(), [](const ModuleSummary& left, const ModuleSummary& right) {
            if (left.mutated_count != right.mutated_count) {
                return left.mutated_count > right.mutated_count;
            }
            if (left.symbol_count != right.symbol_count) {
                return left.symbol_count > right.symbol_count;
            }
            return left.name < right.name;
        });

        out << "| Module | Symbols | Changed Symbols |\n";
        out << "| :--- | ---: | ---: |\n";
        size_t limit = std::min<size_t>(6, modules.size());
        for (size_t i = 0; i < limit; ++i) {
            const ModuleSummary& entry = modules[i];
            out << "| `" << entry.name << "` | " << entry.symbol_count << " | " << entry.mutated_count << " |\n";
        }
        out << "\n";
    }

    out << "## System Status\n\n";
    out << "| Statistic | Value |\n";
    out << "| :--- | :--- |\n";
    out << "| **Analysis Range** | `" << short_sha(report.base_sha) << "` -> `" << short_sha(report.head_sha)
        << "` |\n";
    out << "| **Source Files** | " << report.source_file_count << " |\n";
    out << "| **Changed Files** | " << report.changed_file_count << " |\n";
    out << "| **Graph Density** | " << report.graph_node_count << " symbols, " << report.graph_edge_count
        << " edges |\n";
    out << "| **Impact Radius** | " << report.mutated_nodes.size() << " mutated, " << report.context_nodes.size()
        << " context |\n\n";

    if (!generated_update.empty()) {
        out << "## AI Analysis\n\n";
        out << generated_update << "\n\n";
    }

    out << "## Documentation Artifacts\n\n";
    std::filesystem::path manifest_path = docs_root / config.manifest_file;
    std::filesystem::path context_path = docs_root / config.context_map_file;
    out << "- [" << config.manifest_file << "](" << relative_link(docs_root / config.index_file, manifest_path)
        << ") for machine-readable run metadata.\n";
    out << "- [" << config.context_map_file << "](" << relative_link(docs_root / config.index_file, context_path)
        << ") for a stable summary of key symbols and hotspots.\n\n";

    out << "## Changes & Impact\n\n";
    out << "These symbols were modified in the current diff range.\n\n";
    if (report.mutated_nodes.empty()) {
        out << "No structural changes detected in this range.\n";
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
    out << "These dependencies may be affected by the changes above.\n\n";
    if (report.context_nodes.empty()) {
        out << "No downstream impacts identified.\n";
    } else {
        out << "| Symbol | Type | Relationship |\n";
        out << "| :--- | :--- | :--- |\n";
        for (const Node& node : report.context_nodes) {
            std::filesystem::path symbol_path = docs_root / config.symbols_dir / symbol_filename(node);
            out << "| [" << node.name << "](" << relative_link(docs_root / config.index_file, symbol_path) << ") | ";
            out << type_name(node.type) << " | ";
            out << "Downstream dependency |\n";
        }
    }

    return out.str();
}

std::string render_manifest(const AnalysisReport& report) {
    std::ostringstream out;
    out << "{\n";
    out << "  \"base_sha\": \"" << report.base_sha << "\",\n";
    out << "  \"head_sha\": \"" << report.head_sha << "\",\n";
    out << "  \"source_files\": " << report.source_file_count << ",\n";
    out << "  \"changed_files\": " << report.changed_file_count << ",\n";
    out << "  \"graph_nodes\": " << report.graph_node_count << ",\n";
    out << "  \"graph_edges\": " << report.graph_edge_count << ",\n";
    out << "  \"mutated_symbols\": " << report.mutated_nodes.size() << ",\n";
    out << "  \"context_symbols\": " << report.context_nodes.size() << "\n";
    out << "}\n";
    return out.str();
}

std::string render_context_map(const ContextMap& map) {
    std::ostringstream out;
    out << "{\n";
    out << "  \"schema\": \"archivum-context-map-v1\",\n";
    out << "  \"symbol_count\": " << map.symbol_count << ",\n";
    out << "  \"edge_count\": " << map.edge_count << ",\n";
    out << "  \"top_symbols\": [\n";
    for (size_t i = 0; i < map.top_symbols.size(); ++i) {
        const ContextSymbol& symbol = map.top_symbols[i];
        out << "    {\n";
        out << "      \"name\": \"" << symbol.name << "\",\n";
        out << "      \"type\": \"" << type_name_sentence(symbol.type) << "\",\n";
        out << "      \"location\": \"" << symbol.file_path << ":" << symbol.start_line << "-" << symbol.end_line
            << "\"\n";
        out << "    }";
        if (i + 1 < map.top_symbols.size()) {
            out << ",";
        }
        out << "\n";
    }
    out << "  ],\n";
    out << "  \"hotspot_files\": [\n";
    for (size_t i = 0; i < map.hotspot_files.size(); ++i) {
        const ContextFile& file = map.hotspot_files[i];
        out << "    {\n";
        out << "      \"path\": \"" << file.file_path << "\",\n";
        out << "      \"symbol_count\": " << file.symbol_count << ",\n";
        out << "      \"mutated_count\": " << file.mutated_count << "\n";
        out << "    }";
        if (i + 1 < map.hotspot_files.size()) {
            out << ",";
        }
        out << "\n";
    }
    out << "  ]\n";
    out << "}\n";
    return out.str();
}

}  // namespace

std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report,
                                       const std::filesystem::path& root,
                                       const std::string& existing_docs) {
    std::ostringstream prompt;
    prompt << "You are ArchivumDocs, a world-class Technical Documentation Architect and C++ expert.\n\n";

    if (!config.project_name.empty()) {
        prompt << "Project Name: " << config.project_name << "\n";
    }
    if (!config.project_tagline.empty()) {
        prompt << "Project Tagline: " << config.project_tagline << "\n";
    }
    if (!config.project_name.empty() || !config.project_tagline.empty()) {
        prompt << "\n";
    }

    if (!existing_docs.empty()) {
        prompt << "### CURRENT DOCUMENTATION:\n";
        prompt << "```markdown\n" << existing_docs << "\n```\n\n";
    }

    prompt << "### MISSION:\n";
    prompt << "Your mission is to update the documentation to reflect the latest changes while ensuring a high-quality, comprehensive project overview remains intact.\n\n";

    prompt << "### INSTRUCTIONS:\n";
    prompt << "1. **Analyze existing documentation**: If it already has a good overview, preserve it but update any outdated details. If it's missing or poor, create a professional project overview describing what this project is and its core architecture based on the symbols provided below.\n";
    prompt << "2. **Detailed Impact Analysis**: Analyze the structural changes and write a section on architectural implications. Explain WHY these changes matter.\n";
    prompt << "3. **Merge and Refine**: Do NOT just list changes. Integrate the information into a cohesive narrative. Your output will be used as the 'AI Analysis' section of the main documentation page.\n";
    prompt << "4. **Output Format**: Use clean Markdown with headers and bullet points. Do NOT include YAML frontmatter or code fences for the entire response.\n";
    prompt << "5. **Citations**: When you reference specific files or symbols, include the file path and line range.\n\n";

    prompt << "### NEW ANALYSIS DATA:\n";
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

ContextMap build_context_map(const DependencyGraph& graph, const std::vector<Node>& mutated,
                             const std::vector<Node>& context) {
    ContextMap map;
    map.symbol_count = graph.node_count();
    map.edge_count = graph.edge_count();

    std::unordered_set<NodeId> seen;
    for (const Node& node : mutated) {
        if (seen.size() >= 12) {
            break;
        }
        if (seen.insert(node.id).second) {
            map.top_symbols.push_back({node.name, node.type, node.file_path, node.start_line, node.end_line});
        }
    }
    for (const Node& node : context) {
        if (map.top_symbols.size() >= 12) {
            break;
        }
        if (seen.insert(node.id).second) {
            map.top_symbols.push_back({node.name, node.type, node.file_path, node.start_line, node.end_line});
        }
    }

    std::unordered_map<std::string, ContextFile> file_counts;
    for (const Node& node : context) {
        ContextFile& entry = file_counts[node.file_path];
        entry.file_path = node.file_path;
        ++entry.symbol_count;
    }
    for (const Node& node : mutated) {
        ContextFile& entry = file_counts[node.file_path];
        entry.file_path = node.file_path;
        ++entry.symbol_count;
        ++entry.mutated_count;
    }

    std::vector<ContextFile> files;
    files.reserve(file_counts.size());
    for (const auto& [_, entry] : file_counts) {
        files.push_back(entry);
    }
    std::sort(files.begin(), files.end(), [](const ContextFile& left, const ContextFile& right) {
        if (left.mutated_count != right.mutated_count) {
            return left.mutated_count > right.mutated_count;
        }
        if (left.symbol_count != right.symbol_count) {
            return left.symbol_count > right.symbol_count;
        }
        return left.file_path < right.file_path;
    });

    for (const ContextFile& entry : files) {
        if (map.hotspot_files.size() >= 8) {
            break;
        }
        map.hotspot_files.push_back(entry);
    }

    return map;
}

bool write_context_map(const ArchivumConfig& config, const ContextMap& map, const std::filesystem::path& root,
                       DocumentationWriteResult& result) {
    std::filesystem::path docs_root = root / config.docs_dir;
    std::filesystem::path context_path = docs_root / config.context_map_file;
    if (write_if_changed(context_path, render_context_map(map))) {
        result.changed = true;
        result.written_files.push_back(context_path);
        return true;
    }

    return false;
}

}  // namespace archivum
