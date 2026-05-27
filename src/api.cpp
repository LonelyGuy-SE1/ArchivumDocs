#include <archivum/api.hpp>
#include <cstdlib>

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

}  // namespace archivum
