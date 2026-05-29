#pragma once

#include <archivum/graph.hpp>
#include <cstddef>
#include <optional>
#include <string>
#include <unordered_set>

namespace archivum {

struct DispatchSummary {
    std::string provider;
    bool credentials_available;
    size_t mutated_symbols;
    size_t downstream_symbols;
    size_t context_symbols;
};

struct ProviderRequest {
    std::string provider;
    std::string endpoint;
    std::string model;
    std::string api_key_env;
    std::string reasoning_effort;
    std::string verbosity;
    std::string prompt;
    bool fail_on_error;
};

DispatchSummary summarize_dispatch(const DependencyGraph& graph, const std::unordered_set<NodeId>& mutated,
                                   const std::unordered_set<NodeId>& impacted);

std::optional<std::string> generate_documentation_update(const ProviderRequest& request);

}  // namespace archivum
