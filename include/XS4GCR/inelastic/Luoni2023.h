// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_INELASTIC_LUONI2023_H_
#define INCLUDE_XS4GCR_INELASTIC_LUONI2023_H_

#include <memory>

#include "XS4GCR/inelastic/TotalInelastic.h"

namespace XS4GCR {

class Luoni2023TotalInel : public TotalInelastic {
 public:
  Luoni2023TotalInel() {}

  void print() const override;

  std::shared_ptr<TotalInelastic> clone() override;

  double getTotal(const PID &projectile, const TARGET &target, const double &T_n) const override;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_LUONI2023_H_
