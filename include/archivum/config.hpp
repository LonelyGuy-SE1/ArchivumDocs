#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace archivum {

enum class WriteMode { PULL_REQUEST, AUTO_MERGE, DIRECT, NONE };

struct ArchivumConfig {
    std::string project_name;
    std::string project_tagline;
    std::string docs_dir = "docs";
    std::string index_file = "index.md";
    std::string symbols_dir = "symbols";
    std::string manifest_file = "archivum-manifest.json";
    std::string context_map_file = "archivum-context-map.json";
    std::string provider = "auto";
    std::string endpoint = "https://api.openai.com/v1/responses";
    std::string model = "gpt-5.5";
    std::string api_key_env = "OPENAI_API_KEY";
    std::string reasoning_effort = "low";
    std::string verbosity = "low";
    WriteMode write_mode = WriteMode::PULL_REQUEST;
    size_t max_context_bytes = 180000;
    size_t max_symbols = 250;
    bool fail_on_provider_error = false;
    bool update_docs = true;
    std::vector<std::string> ignore_dirs = {".git", ".github", ".vscode", "build", "docs"};
};

ArchivumConfig load_config(const std::filesystem::path& path);
std::string write_mode_name(WriteMode mode);

}  // namespace archivum
