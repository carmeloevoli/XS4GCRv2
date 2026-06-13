// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_FRAGMENTATIONS_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_FRAGMENTATIONS_H_

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

using channel = FragmentationChannel;

class Fragmentation : public XSecModel<Fragmentation> {
 public:
  virtual bool hasChannel(const FragmentationChannel& ch) const = 0;

  // Cumulative production cross-section for the fragmentation channel ch on a target at
  // kinetic energy per nucleon T_n. When do_ghosts is true, short-lived "ghost" parents
  // feeding the daughter are folded in.
  virtual double getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n,
                     bool do_ghosts = true) = 0;

  inline void set_model_name(const std::string& m) { model_name = m; }

 protected:
  std::string model_name;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_FRAGMENTATIONS_H_
