#ifndef INCLUDE_XS4GCR_TERTIARY_H
#define INCLUDE_XS4GCR_TERTIARY_H

#include <memory>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class TertiaryProtons : public XSecModel<TertiaryProtons> {
 public:
  // Differential cross-section for a tertiary proton carrying fraction x = T_sec/T_proj
  // of a projectile of kinetic energy per nucleon T_proj on a target.
  virtual double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_TERTIARY_H
