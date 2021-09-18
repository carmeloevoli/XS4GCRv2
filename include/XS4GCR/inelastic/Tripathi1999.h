// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_INELASTIC_TRIPATHI99_H_
#define INCLUDE_XS4GCR_INELASTIC_TRIPATHI99_H_

#include <memory>

#include "XS4GCR/inelastic/TotalInelastic.h"

namespace XS4GCR {

class Tripathi1999TotalInel : public TotalInelastic {
 public:
  Tripathi1999TotalInel() {}

  void print() const override;

  std::shared_ptr<TotalInelastic> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n) const override;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_TRIPATHI99_H_
