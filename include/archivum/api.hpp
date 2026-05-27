#pragma once

#include <archivum/graph.hpp>
#include <cstddef>
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

DispatchSummary summarize_dispatch(const DependencyGraph& graph, const std::unordered_set<NodeId>& mutated,
                                   const std::unordered_set<NodeId>& impacted);

}  // namespace archivum
