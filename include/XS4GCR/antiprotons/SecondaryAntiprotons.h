// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_ANTIPROTONS_SECONDARYANTIPROTONS_H
#define INCLUDE_XS4GCR_ANTIPROTONS_SECONDARYANTIPROTONS_H

#include <memory>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class SecondaryAntiprotons : public XSecModel<SecondaryAntiprotons> {
 public:
  // Differential cross-section for a secondary antiproton carrying fraction x = T_sec/T_proj
  // of a projectile of kinetic energy per nucleon T_proj on a target.
  virtual double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const = 0;

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
