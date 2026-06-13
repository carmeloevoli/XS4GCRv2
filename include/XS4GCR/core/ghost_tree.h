// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_GHOST_TREE_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_GHOST_TREE_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

using GhostParent = std::pair<PID, double>;
using GhostParents = std::vector<GhostParent>;
using GhostTreeMap = std::map<PID, GhostParents>;

class GhostTree {
 public:
  GhostTree() = default;
  explicit GhostTree(const std::string& filename);

  bool hasChild(const PID& child) const;
  size_t nParents(const PID& child) const;
  const GhostParents& getParents(const PID& child) const;

 private:
  bool isGhost(double halfLifeValue, const std::string& halfLifeUnits) const;
  void readGhosts();

  std::string filename;
  GhostTreeMap tree;
  double halfLifeMax = cgs::kyr;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_GHOST_TREE_H_
