#ifndef INCLUDE_XS4GCR_TERTIARY_H
#define INCLUDE_XS4GCR_TERTIARY_H

#include <memory>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class TertiaryProtons {
 public:
  virtual ~TertiaryProtons() {}

  virtual void print() const = 0;

  virtual std::shared_ptr<TertiaryProtons> clone() = 0;

  virtual double get(const PID &projectile, const TARGET &target, const double &T_proj,
                     const double &T_proton) const = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LEPTONS_LEPTONS_H
