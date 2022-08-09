#ifndef INCLUDE_XS4GCR_GAMMAS_PI0GAMMAS_H
#define INCLUDE_XS4GCR_GAMMAS_PI0GAMMAS_H

#include <memory>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class Pi0Gammas {
 public:
  virtual ~Pi0Gammas() {}

  virtual void print() const = 0;

  virtual std::shared_ptr<Pi0Gammas> clone() = 0;

  virtual double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_g) const = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LEPTONS_LEPTONS_H
