#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace archivum {
using NodeId = uint64_t;

enum class NodeType { FUNCTION, CLASS, STRUCT, METHOD, UNKNOWN };

struct Node {
    NodeId id;
    std::string name;
    std::string file_path;
    std::string signature;
    std::vector<std::string> references;

    uint32_t start_line;
    uint32_t end_line;
    NodeType type;

    uint64_t interface_hash;
    uint64_t source_hash;
};

class DependencyGraph {
    std::unordered_map<NodeId, Node> registry;
    std::unordered_map<NodeId, std::unordered_set<NodeId>> downstream_edges;

   public:
    DependencyGraph() = default;

    void register_node(const Node& node);
    void add_dependency(NodeId provider, NodeId consumer);
    void infer_dependencies();

    std::unordered_set<NodeId> calculate_blast_radius(NodeId origin_id) const;
    bool contains(NodeId id) const;
    const Node& get_node(NodeId id) const;
    std::vector<Node> nodes() const;
    std::vector<NodeId> downstream(NodeId id) const;
    size_t node_count() const;
    size_t edge_count() const;
};
}  // namespace archivum
