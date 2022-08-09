#ifndef INCLUDE_XS4GCR_INELASTIC_TOTALINELASTIC_H
#define INCLUDE_XS4GCR_INELASTIC_TOTALINELASTIC_H

#include <memory>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class TotalInelastic {
 public:
  virtual ~TotalInelastic() = default;

  virtual void print() const = 0;

  virtual std::shared_ptr<TotalInelastic> clone() = 0;

  virtual double get(const PID &projectile, const TARGET &target, const double &T_n) const = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_TOTALINELASTIC_H
