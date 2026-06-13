#ifndef INCLUDE_XS4GCR_LEPTONS_LEPTONS_H
#define INCLUDE_XS4GCR_LEPTONS_LEPTONS_H

#include <memory>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class SecondaryLeptons : public XSecModel<SecondaryLeptons> {
 public:
  // Differential cross-section for a secondary lepton carrying fraction x = T_sec/T_proj
  // of a projectile of kinetic energy per nucleon T_proj on a target.
  virtual double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const = 0;

  virtual double annihilationXsec(const PID &target, const double &T_electron) const;

 protected:
  PID m_lepton;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LEPTONS_LEPTONS_H
