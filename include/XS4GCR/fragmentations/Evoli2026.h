// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_EVOLI2026_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_EVOLI2026_H_

#include <memory>
#include <set>
#include <string>

#include "XS4GCR/core/frag_channels_table.h"
#include "XS4GCR/core/ghost_tree.h"
#include "XS4GCR/fragmentations/fragmentations.h"

namespace XS4GCR {

enum class Evoli2026FallbackModel { W93, ST99 };

class Evoli2026 : public Fragmentation {
 public:
  explicit Evoli2026(Evoli2026FallbackModel fallbackModel = Evoli2026FallbackModel::W93);

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

  std::string directChannelsFilename = "data/Evoli2026/direct_channels.txt";
  std::string ghostListFilename = "data/Evoli2026/ghost_list_IAEA2026.txt";

  Evoli2026FallbackModel fallbackModel;
  FragmentationChannels directChannels;
  GhostTree ghostTree;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_EVOLI2026_H_
