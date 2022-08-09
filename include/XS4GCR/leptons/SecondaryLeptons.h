#ifndef INCLUDE_XS4GCR_LEPTONS_LEPTONS_H
#define INCLUDE_XS4GCR_LEPTONS_LEPTONS_H

#include <memory>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class SecondaryLeptons {
 public:
  virtual ~SecondaryLeptons() {}

  virtual void print() const = 0;

  virtual std::shared_ptr<SecondaryLeptons> clone() = 0;

  virtual double get(const PID &projectile, const TARGET &target, const double &T_proj,
                     const double &T_lepton) const = 0;

  virtual double annihilationXsec(const PID &target, const double &T_electron) const;

 protected:
  PID m_lepton;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LEPTONS_LEPTONS_H
