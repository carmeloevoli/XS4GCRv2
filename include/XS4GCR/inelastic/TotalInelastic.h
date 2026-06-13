#ifndef INCLUDE_XS4GCR_INELASTIC_TOTALINELASTIC_H
#define INCLUDE_XS4GCR_INELASTIC_TOTALINELASTIC_H

#include <memory>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class TotalInelastic : public XSecModel<TotalInelastic> {
 public:
  // Total inelastic cross-section for a projectile on a target at kinetic energy
  // per nucleon T_n.
  virtual double getTotal(const PID &projectile, const TARGET &target, const double &T_n) const = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_TOTALINELASTIC_H
