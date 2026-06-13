// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_EVOLI2026_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_EVOLI2026_H_

#include <memory>
#include <set>
#include <string>

#include "XS4GCR/core/fit_data.h"
#include "XS4GCR/core/fitting_functions.h"
#include "XS4GCR/fragmentations/fragmentations.h"
#include "XS4GCR/core/ghost_tree.h"

namespace XS4GCR {

class Evoli2026 : public Fragmentation {
 public:
  Evoli2026();

  void print() const override;
  std::shared_ptr<Fragmentation> clone() override;
  bool hasChannel(const FragmentationChannel& ch) const override;
  double getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n,
                  bool do_ghosts = true) override;

 private:
  void init();
  bool hasChannel(const FragmentationChannel& ch, std::set<PID> activeGhosts) const;
  double direct(const FragmentationChannel& ch, double T_n) const;
  double withGhosts(const FragmentationChannel& ch, double T_n) const;
  double withGhosts(const FragmentationChannel& ch, double T_n, std::set<PID> activeGhosts) const;
  double bestfitNormalization(const FragmentationChannel& ch) const;

  std::string ghostListFilename = "data/Evoli2026/ghost_list.txt";
  std::string fitParamsFilename = "data/Evoli2026/fragmentation_direct_fit_params.txt";
  std::string fitDataFilename = "data/Evoli2026/xsecs_data_full.txt";

  FittingFunctions fittingFunctions;
  GhostTree ghostTree;
  FitData fitData;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_EVOLI2026_H_
