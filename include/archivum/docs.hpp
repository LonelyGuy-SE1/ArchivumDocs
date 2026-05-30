#pragma once

#include <archivum/config.hpp>
#include <archivum/git_utils.hpp>
#include <archivum/graph.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace archivum {

struct AnalysisReport {
    std::string base_sha;
    std::string head_sha;
    std::string range_source;
    bool initial_scan = false;
    size_t source_file_count = 0;
    size_t changed_file_count = 0;
    size_t graph_node_count = 0;
    size_t graph_edge_count = 0;
    std::vector<FileDiff> diffs;
    std::vector<Node> mutated_nodes;
    std::vector<Node> context_nodes;
};

struct DocumentationWriteResult {
    bool changed = false;
    std::vector<std::filesystem::path> written_files;
};

struct ContextSymbol {
    std::string name;
    NodeType type = NodeType::UNKNOWN;
    std::string file_path;
    uint32_t start_line = 0;
    uint32_t end_line = 0;
};

struct ContextFile {
    std::string file_path;
    size_t symbol_count = 0;
    size_t mutated_count = 0;
};

struct ContextMap {
    size_t symbol_count = 0;
    size_t edge_count = 0;
    std::vector<ContextSymbol> top_symbols;
    std::vector<ContextFile> hotspot_files;
};

std::string build_documentation_prompt(const ArchivumConfig& config, const AnalysisReport& report,
                                       const std::filesystem::path& root,
                                       const std::string& existing_docs);

DocumentationWriteResult write_documentation(const ArchivumConfig& config, const AnalysisReport& report,
                                             const std::string& generated_update, const std::filesystem::path& root);

ContextMap build_context_map(const DependencyGraph& graph, const std::vector<Node>& mutated,
                             const std::vector<Node>& context);

bool write_context_map(const ArchivumConfig& config, const ContextMap& map, const std::filesystem::path& root,
                       DocumentationWriteResult& result);

}  // namespace archivum
