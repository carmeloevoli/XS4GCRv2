// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_INELASTIC_LETAW1983_H
#define INCLUDE_XS4GCR_INELASTIC_LETAW1983_H

#include <memory>

#include "XS4GCR/inelastic/inelastic.h"
#include "XS4GCR/pid.h"

namespace XS4GCR {

class Letaw1983TotalInel : public TotalInelastic {
 public:
  Letaw1983TotalInel() {}

  void print() const override;

  std::shared_ptr<TotalInelastic> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n) const override;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_LETAW1983_H
