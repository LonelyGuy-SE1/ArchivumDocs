#pragma once

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

  uint32_t start_line;
  uint32_t end_line;
  NodeType type;

  uint64_t interface_hash;
};

class DependencyGraph {
  std::unordered_map<NodeId, Node> registry;
  std::unordered_map<NodeId, std::vector<NodeId>> downstream_edges;

public:
  DependencyGraph() = default;

  void register_node(const Node &node);
  void add_dependency(NodeId provider, NodeId consumer);

  std::unordered_set<NodeId> calculate_blast_radius(NodeId origin_id) const;
  bool contains(NodeId id) const;
  const Node &get_node(NodeId id) const;
};
} // namespace archivum