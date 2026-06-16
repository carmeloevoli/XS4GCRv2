// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_CORE_FRAG_CHANNELS_TABLE_H_
#define INCLUDE_XS4GCR_CORE_FRAG_CHANNELS_TABLE_H_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

using CrossSectionVector = std::vector<double>;
using FragmentationChannelTable = std::map<FragmentationChannel, CrossSectionVector>;

class FragmentationChannels {
 public:
  FragmentationChannels() = default;
  explicit FragmentationChannels(const std::string& filename);

  bool hasChannel(const FragmentationChannel& ch) const;
  const CrossSectionVector& getCrossSections(const FragmentationChannel& ch) const;
  const std::vector<double>& getEnergies() const;
  const std::vector<double>& getLogEnergies() const;
  size_t size() const;

 private:
  void readDataFile();

  std::string filename;
  std::vector<double> energies;
  std::vector<double> logEnergies;
  FragmentationChannelTable channels;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_CORE_FRAG_CHANNELS_TABLE_H_
