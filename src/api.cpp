#include <archivum/api.hpp>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace archivum {

namespace {

bool present(const char* value) { return value != nullptr && value[0] != '\0'; }

std::string provider_name() {
    if (present(std::getenv("ARCHIVUM_LLM_ENDPOINT"))) {
        return "custom";
    }
    if (present(std::getenv("OPENAI_API_KEY"))) {
        return "openai";
    }
    if (present(std::getenv("ANTHROPIC_API_KEY"))) {
        return "anthropic";
    }
    return "shadow";
}

std::string json_escape(const std::string& value) {
    std::ostringstream out;
    for (unsigned char c : value) {
        switch (c) {
            case '\\':
                out << "\\\\";
                break;
            case '"':
                out << "\\\"";
                break;
            case '\n':
                out << "\\n";
                break;
            case '\r':
                out << "\\r";
                break;
            case '\t':
                out << "\\t";
                break;
            default:
                if (c < 0x20) {
                    out << "\\u";
                    out << "00";
                    constexpr char hex[] = "0123456789abcdef";
                    out << hex[(c >> 4) & 0x0f] << hex[c & 0x0f];
                } else {
                    out << static_cast<char>(c);
                }
        }
    }
    return out.str();
}

std::string json_unescape(const std::string& value) {
    std::string out;
    out.reserve(value.size());

    for (size_t i = 0; i < value.size(); ++i) {
        char c = value[i];
        if (c != '\\' || i + 1 >= value.size()) {
            out.push_back(c);
            continue;
        }

        char escaped = value[++i];
        switch (escaped) {
            case 'n':
                out.push_back('\n');
                break;
            case 'r':
                out.push_back('\r');
                break;
            case 't':
                out.push_back('\t');
                break;
            case '"':
            case '\\':
            case '/':
                out.push_back(escaped);
                break;
            default:
                out.push_back(escaped);
                break;
        }
    }

    return out;
}

std::optional<std::string> extract_json_string_after(const std::string& body, size_t offset, const std::string& key) {
    size_t key_pos = body.find(key, offset);
    if (key_pos == std::string::npos) {
        return std::nullopt;
    }

    size_t colon = body.find(':', key_pos + key.size());
    if (colon == std::string::npos) {
        return std::nullopt;
    }

    size_t quote = body.find('"', colon + 1);
    if (quote == std::string::npos) {
        return std::nullopt;
    }

    std::string encoded;
    bool escaped = false;
    for (size_t i = quote + 1; i < body.size(); ++i) {
        char c = body[i];
        if (escaped) {
            encoded.push_back('\\');
            encoded.push_back(c);
            escaped = false;
            continue;
        }
        if (c == '\\') {
            escaped = true;
            continue;
        }
        if (c == '"') {
            return json_unescape(encoded);
        }
        encoded.push_back(c);
    }

    return std::nullopt;
}

std::optional<std::string> extract_response_text(const std::string& body) {
    size_t output_text = body.find("\"output_text\"");
    if (output_text != std::string::npos) {
        std::optional<std::string> text = extract_json_string_after(body, output_text, "\"text\"");
        if (text.has_value()) {
            return text;
        }
    }

    size_t message_content = body.find("\"content\"");
    if (message_content != std::string::npos) {
        return extract_json_string_after(body, message_content, "\"content\"");
    }

    return std::nullopt;
}

std::filesystem::path temp_file(const std::string& name) {
    auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() / ("archivum-" + name + "-" + std::to_string(stamp));
}

std::string shell_quote(const std::filesystem::path& path) {
    std::string value = path.string();
    std::string quoted = "\"";
    for (char c : value) {
        if (c == '"' || c == '\\') {
            quoted.push_back('\\');
            quoted.push_back(c);
        } else {
            quoted.push_back(c);
        }
    }
    quoted.push_back('"');
    return quoted;
}

std::string run_capture(const std::string& command) {
#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
#else
    FILE* pipe = popen(command.c_str(), "r");
#endif
    if (pipe == nullptr) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Failed to start provider transport.");
    }

    std::array<char, 4096> buffer{};
    std::string output;
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        output += buffer.data();
    }

#ifdef _WIN32
    int code = _pclose(pipe);
#else
    int code = pclose(pipe);
#endif
    if (code != 0) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Provider transport exited with a non-zero status.");
    }

    return output;
}

std::string request_payload(const ProviderRequest& request) {
    std::ostringstream out;
    out << "{";
    out << "\"model\":\"" << json_escape(request.model) << "\",";
    out << "\"input\":\"" << json_escape(request.prompt) << "\",";
    out << "\"reasoning\":{\"effort\":\"" << json_escape(request.reasoning_effort) << "\"},";
    out << "\"text\":{\"verbosity\":\"" << json_escape(request.verbosity) << "\"}";
    out << "}";
    return out.str();
}

}  // namespace

DispatchSummary summarize_dispatch(const DependencyGraph& graph, const std::unordered_set<NodeId>& mutated,
                                   const std::unordered_set<NodeId>& impacted) {
    DispatchSummary summary;
    summary.provider = provider_name();
    summary.credentials_available = summary.provider != "shadow";
    summary.mutated_symbols = mutated.size();
    summary.downstream_symbols = impacted.size() > mutated.size() ? impacted.size() - mutated.size() : 0;
    summary.context_symbols = 0;

    for (NodeId id : impacted) {
        if (graph.contains(id)) {
            ++summary.context_symbols;
        }
    }

    return summary;
}

std::optional<std::string> generate_documentation_update(const ProviderRequest& request) {
    const char* key = std::getenv(request.api_key_env.c_str());
    if (!present(key) || request.provider == "shadow" || request.provider == "none") {
        return std::nullopt;
    }

    std::filesystem::path body_path = temp_file("request.json");
    std::filesystem::path config_path = temp_file("curl.cfg");

    try {
        {
            std::ofstream body(body_path, std::ios::binary);
            body << request_payload(request);
        }
        {
            std::ofstream config(config_path, std::ios::binary);
            config << "silent\n";
            config << "show-error\n";
            config << "fail-with-body\n";
            config << "request = POST\n";
            config << "url = \"" << request.endpoint << "\"\n";
            config << "header = \"Content-Type: application/json\"\n";
            config << "header = \"Authorization: Bearer " << key << "\"\n";
            config << "data-binary = \"@" << body_path.generic_string() << "\"\n";
        }

        std::string response = run_capture("curl --config " + shell_quote(config_path));
        std::filesystem::remove(body_path);
        std::filesystem::remove(config_path);
        std::optional<std::string> text = extract_response_text(response);
        if (text.has_value()) {
            return text;
        }
        if (request.fail_on_error) {
            throw std::runtime_error("[ArchivumDocs] FATAL: Provider response did not contain text output.");
        }
    } catch (...) {
        std::filesystem::remove(body_path);
        std::filesystem::remove(config_path);
        if (request.fail_on_error) {
            throw;
        }
    }

    return std::nullopt;
}

}  // namespace archivum
