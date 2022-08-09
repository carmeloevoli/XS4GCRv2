// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_ANTIPROTONS_SECONDARYANTIPROTONS_H
#define INCLUDE_XS4GCR_ANTIPROTONS_SECONDARYANTIPROTONS_H

#include <memory>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class SecondaryAntiprotons {
 public:
  virtual ~SecondaryAntiprotons() {}

  virtual void print() const = 0;

  virtual std::shared_ptr<SecondaryAntiprotons> clone() = 0;

  virtual double get(const PID &projectile, const TARGET &target, const double &T_n, const double &T_ap) const = 0;

  double getTotalInelastic(const TARGET &target, const double &T_ap);

  double getAnnihilatingInelastic(const TARGET &target, const double &T_ap);

  double getNonAnnihilatingInelastic(const TARGET &target, const double &T_ap);

 protected:
  const double alpha = 1. / 3.;
  const double a[4] = {-107.9, 29.43, -1.655, 189.9};
  const double b[4] = {-100.5, 20.37, -1.182, 162.0};
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_ANTIPROTONS_ANTIPROTONS_H_
