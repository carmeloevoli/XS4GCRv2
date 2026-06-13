// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/core/ghost_tree.h"

#include <plog/Log.h>

#include <fstream>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>

namespace {

int elementZ(const std::string& symbol) {
  static const std::map<std::string, int> elements = {
      {"H", 1},   {"He", 2},  {"Li", 3}, {"Be", 4}, {"B", 5},   {"C", 6},  {"N", 7},  {"O", 8},
      {"F", 9},   {"Ne", 10}, {"Na", 11}, {"Mg", 12}, {"Al", 13}, {"Si", 14}, {"P", 15}, {"S", 16},
      {"Cl", 17}, {"Ar", 18}, {"K", 19}, {"Ca", 20}, {"Sc", 21}, {"Ti", 22}, {"V", 23}, {"Cr", 24},
      {"Mn", 25}, {"Fe", 26}, {"Co", 27}, {"Ni", 28}, {"Cu", 29}, {"Zn", 30},
  };

  const auto it = elements.find(symbol);
  if (it == elements.end()) throw std::runtime_error("unknown element symbol in ghost list: " + symbol);
  return it->second;
}

}  // namespace

namespace XS4GCR {

GhostTree::GhostTree(const std::string& filename) : filename(filename) { readGhosts(); }

bool GhostTree::hasChild(const PID& child) const { return tree.find(child) != tree.end(); }

size_t GhostTree::nParents(const PID& child) const {
  const auto it = tree.find(child);
  return (it == tree.end()) ? 0 : it->second.size();
}

const GhostParents& GhostTree::getParents(const PID& child) const { return tree.at(child); }

bool GhostTree::isGhost(double halfLifeValue, const std::string& halfLifeUnits) const {
  double halfLife = halfLifeValue;
  if (halfLifeUnits == "ms")
    halfLife *= 1e-3 * cgs::second;
  else if (halfLifeUnits == "s")
    halfLife *= cgs::second;
  else if (halfLifeUnits == "m")
    halfLife *= 60. * cgs::second;
  else if (halfLifeUnits == "h")
    halfLife *= 3600. * cgs::second;
  else if (halfLifeUnits == "d")
    halfLife *= 24. * 3600. * cgs::second;
  else if (halfLifeUnits == "y")
    halfLife *= cgs::year;
  else
    throw std::runtime_error("unknown half-life unit in ghost list: " + halfLifeUnits);

  return halfLife < halfLifeMax;
}

void GhostTree::readGhosts() {
  std::ifstream input(filename.c_str());
  if (!input) throw std::runtime_error("problem with opening Evoli2026 ghost list: " + filename);

  input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::string childSymbol;
  std::string parentSymbol;
  std::string halfLifeUnits;
  int childA = 0;
  int parentA = 0;
  double branching = 0.;
  double halfLifeValue = 0.;
  size_t counter = 0;

  while (input >> parentSymbol >> parentA >> childSymbol >> childA >> branching >> halfLifeValue >> halfLifeUnits) {
    if (!isGhost(halfLifeValue, halfLifeUnits)) continue;

    const PID child(elementZ(childSymbol), childA);
    const GhostParent parent = std::make_pair(PID(elementZ(parentSymbol), parentA), branching / 100.);
    tree[child].push_back(parent);
    ++counter;
  }

  LOGI << "read " << counter << " Evoli2026 ghost reactions";
}

}  // namespace XS4GCR
