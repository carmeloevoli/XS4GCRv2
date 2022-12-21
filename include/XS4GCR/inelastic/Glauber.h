// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_INELASTIC_GLAUBER_H_
#define INCLUDE_XS4GCR_INELASTIC_GLAUBER_H_

#include <memory>

#include "XS4GCR/inelastic/TotalInelastic.h"

namespace XS4GCR {

class GlauberTotalInel : public TotalInelastic {
 public:
  GlauberTotalInel() {}

  void print() const override;

  std::shared_ptr<TotalInelastic> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n) const override;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_GLAUBER_H_
