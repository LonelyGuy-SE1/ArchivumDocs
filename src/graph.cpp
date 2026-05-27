#include <algorithm>
#include <archivum/graph.hpp>
#include <queue>
#include <stdexcept>

namespace archivum {

namespace {

std::string symbol_key(const std::string& value) {
    size_t template_start = value.find('<');
    std::string trimmed = value.substr(0, template_start);
    size_t scope = trimmed.rfind("::");
    if (scope != std::string::npos) {
        return trimmed.substr(scope + 2);
    }
    return trimmed;
}

}  // namespace

void DependencyGraph::register_node(const Node& node) { registry[node.id] = node; }

void DependencyGraph::add_dependency(NodeId provider, NodeId consumer) {
    if (provider != consumer && registry.find(provider) != registry.end() &&
        registry.find(consumer) != registry.end()) {
        downstream_edges[provider].insert(consumer);
    }
}

void DependencyGraph::infer_dependencies() {
    downstream_edges.clear();

    std::unordered_map<std::string, std::vector<NodeId>> providers;
    providers.reserve(registry.size() * 2);

    for (const auto& [id, node] : registry) {
        providers[node.name].push_back(id);
        providers[symbol_key(node.name)].push_back(id);
    }

    for (const auto& [consumer_id, consumer] : registry) {
        std::unordered_set<NodeId> linked;
        for (const std::string& reference : consumer.references) {
            auto it = providers.find(symbol_key(reference));
            if (it == providers.end()) {
                it = providers.find(reference);
            }
            if (it == providers.end()) {
                continue;
            }
            for (NodeId provider_id : it->second) {
                if (provider_id != consumer_id && linked.insert(provider_id).second) {
                    downstream_edges[provider_id].insert(consumer_id);
                }
            }
        }
    }
}

std::unordered_set<NodeId> DependencyGraph::calculate_blast_radius(NodeId origin_id) const {
    std::unordered_set<NodeId> blast_radius;

    if (registry.find(origin_id) == registry.end()) {
        return blast_radius;
    }

    std::queue<NodeId> frontier;
    frontier.push(origin_id);
    blast_radius.insert(origin_id);

    while (!frontier.empty()) {
        NodeId current = frontier.front();
        frontier.pop();

        auto edge_it = downstream_edges.find(current);
        if (edge_it != downstream_edges.end()) {
            for (NodeId consumer : edge_it->second) {
                if (blast_radius.find(consumer) == blast_radius.end()) {
                    blast_radius.insert(consumer);
                    frontier.push(consumer);
                }
            }
        }
    }

    return blast_radius;
}

bool DependencyGraph::contains(NodeId id) const { return registry.find(id) != registry.end(); }

const Node& DependencyGraph::get_node(NodeId id) const {
    auto it = registry.find(id);
    if (it != registry.end()) {
        return it->second;
    }
    throw std::runtime_error("[ArchivumDocs] FATAL: Attempted to access unregistered Node ID.");
}

std::vector<Node> DependencyGraph::nodes() const {
    std::vector<Node> values;
    values.reserve(registry.size());
    for (const auto& [id, node] : registry) {
        values.push_back(node);
    }
    std::sort(values.begin(), values.end(), [](const Node& left, const Node& right) {
        if (left.file_path != right.file_path) {
            return left.file_path < right.file_path;
        }
        if (left.start_line != right.start_line) {
            return left.start_line < right.start_line;
        }
        return left.name < right.name;
    });
    return values;
}

std::vector<NodeId> DependencyGraph::downstream(NodeId id) const {
    std::vector<NodeId> values;
    auto it = downstream_edges.find(id);
    if (it == downstream_edges.end()) {
        return values;
    }
    values.assign(it->second.begin(), it->second.end());
    std::sort(values.begin(), values.end());
    return values;
}

size_t DependencyGraph::node_count() const { return registry.size(); }

size_t DependencyGraph::edge_count() const {
    size_t total = 0;
    for (const auto& [id, edges] : downstream_edges) {
        total += edges.size();
    }
    return total;
}

}  // namespace archivum
