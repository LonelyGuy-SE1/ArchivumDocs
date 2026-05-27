#include <archivum/graph.hpp>
#include <queue>
#include <stdexcept>

namespace archivum {

void DependencyGraph::register_node(const Node& node) { registry[node.id] = node; }

void DependencyGraph::add_dependency(NodeId provider, NodeId consumer) {
    downstream_edges[provider].push_back(consumer);
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

}  // namespace archivum