#include <algorithm>
#include <archivum/config.hpp>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace archivum {

namespace {

std::string trim(std::string value) {
    auto keep = [](unsigned char c) { return !std::isspace(c); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), keep));
    value.erase(std::find_if(value.rbegin(), value.rend(), keep).base(), value.end());
    return value;
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::string unquote(std::string value) {
    value = trim(value);
    if (value.size() >= 2 &&
        ((value.front() == '"' && value.back() == '"') || (value.front() == '\'' && value.back() == '\''))) {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

bool parse_bool(const std::string& value) {
    std::string normalized = lower(trim(value));
    return normalized == "true" || normalized == "yes" || normalized == "1" || normalized == "on";
}

size_t parse_size(const std::string& key, const std::string& value) {
    try {
        return static_cast<size_t>(std::stoull(trim(value)));
    } catch (const std::exception&) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Invalid numeric config value for " + key);
    }
}

WriteMode parse_write_mode(const std::string& value) {
    std::string normalized = lower(trim(value));
    if (normalized == "pull_request" || normalized == "pr") {
        return WriteMode::PULL_REQUEST;
    }
    if (normalized == "auto_merge") {
        return WriteMode::AUTO_MERGE;
    }
    if (normalized == "direct" || normalized == "commit") {
        return WriteMode::DIRECT;
    }
    if (normalized == "none" || normalized == "shadow") {
        return WriteMode::NONE;
    }
    throw std::runtime_error("[ArchivumDocs] FATAL: Unknown write mode: " + value);
}

void apply_pair(ArchivumConfig& config, std::string key, const std::string& value) {
    key = lower(trim(key));
    std::string parsed = unquote(value);

    if (key == "docs_dir" || key == "docs.dir") {
        config.docs_dir = parsed;
    } else if (key == "index_file" || key == "docs.index_file") {
        config.index_file = parsed;
    } else if (key == "symbols_dir" || key == "docs.symbols_dir") {
        config.symbols_dir = parsed;
    } else if (key == "manifest_file" || key == "docs.manifest_file") {
        config.manifest_file = parsed;
    } else if (key == "provider" || key == "ai.provider") {
        config.provider = lower(parsed);
    } else if (key == "endpoint" || key == "ai.endpoint") {
        config.endpoint = parsed;
    } else if (key == "model" || key == "ai.model") {
        config.model = parsed;
    } else if (key == "api_key_env" || key == "ai.api_key_env") {
        config.api_key_env = parsed;
    } else if (key == "reasoning_effort" || key == "ai.reasoning_effort") {
        config.reasoning_effort = lower(parsed);
    } else if (key == "verbosity" || key == "ai.verbosity") {
        config.verbosity = lower(parsed);
    } else if (key == "write_mode" || key == "mode") {
        config.write_mode = parse_write_mode(parsed);
    } else if (key == "max_context_bytes" || key == "limits.max_context_bytes") {
        config.max_context_bytes = parse_size(key, parsed);
    } else if (key == "max_symbols" || key == "limits.max_symbols") {
        config.max_symbols = parse_size(key, parsed);
    } else if (key == "fail_on_provider_error" || key == "ai.fail_on_error") {
        config.fail_on_provider_error = parse_bool(parsed);
    } else if (key == "update_docs" || key == "docs.update") {
        config.update_docs = parse_bool(parsed);
    }
}

void apply_env(ArchivumConfig& config) {
    const std::unordered_map<std::string, std::function<void(const std::string&)>> handlers = {
        {"ARCHIVUM_DOCS_DIR", [&](const std::string& value) { config.docs_dir = value; }},
        {"ARCHIVUM_PROVIDER", [&](const std::string& value) { config.provider = lower(value); }},
        {"ARCHIVUM_LLM_ENDPOINT", [&](const std::string& value) { config.endpoint = value; }},
        {"ARCHIVUM_MODEL", [&](const std::string& value) { config.model = value; }},
        {"ARCHIVUM_API_KEY_ENV", [&](const std::string& value) { config.api_key_env = value; }},
        {"ARCHIVUM_WRITE_MODE", [&](const std::string& value) { config.write_mode = parse_write_mode(value); }},
        {"ARCHIVUM_MAX_CONTEXT_BYTES",
         [&](const std::string& value) { config.max_context_bytes = parse_size("ARCHIVUM_MAX_CONTEXT_BYTES", value); }},
        {"ARCHIVUM_MAX_SYMBOLS",
         [&](const std::string& value) { config.max_symbols = parse_size("ARCHIVUM_MAX_SYMBOLS", value); }},
        {"ARCHIVUM_UPDATE_DOCS", [&](const std::string& value) { config.update_docs = parse_bool(value); }},
        {"ARCHIVUM_FAIL_ON_PROVIDER_ERROR",
         [&](const std::string& value) { config.fail_on_provider_error = parse_bool(value); }}};

    for (const auto& [name, handler] : handlers) {
        const char* value = std::getenv(name.c_str());
        if (value != nullptr && value[0] != '\0') {
            handler(value);
        }
    }
}

}  // namespace

ArchivumConfig load_config(const std::filesystem::path& path) {
    ArchivumConfig config;
    std::ifstream file(path);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::string cleaned = trim(line);
            if (cleaned.empty() || cleaned.front() == '#') {
                continue;
            }

            size_t delimiter = cleaned.find(':');
            size_t equals = cleaned.find('=');
            if (delimiter == std::string::npos || (equals != std::string::npos && equals < delimiter)) {
                delimiter = equals;
            }
            if (delimiter == std::string::npos) {
                continue;
            }

            apply_pair(config, cleaned.substr(0, delimiter), cleaned.substr(delimiter + 1));
        }
    }

    apply_env(config);
    return config;
}

std::string write_mode_name(WriteMode mode) {
    switch (mode) {
        case WriteMode::PULL_REQUEST:
            return "pull_request";
        case WriteMode::AUTO_MERGE:
            return "auto_merge";
        case WriteMode::DIRECT:
            return "direct";
        case WriteMode::NONE:
            return "none";
    }
    return "none";
}

}  // namespace archivum
