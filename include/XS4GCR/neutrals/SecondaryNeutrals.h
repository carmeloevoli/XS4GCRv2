#ifndef INCLUDE_XS4GCR_NEUTRALS_SECONDARYNEUTRALS_H
#define INCLUDE_XS4GCR_NEUTRALS_SECONDARYNEUTRALS_H

#include <memory>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

enum class NeutralParticleType { GAMMA, ALLNUS };

class SecondaryNeutrals : public XSecModel<SecondaryNeutrals> {
 public:
  // Differential cross-section for a neutral secondary (gamma or neutrino) carrying
  // fraction x = T_sec/T_proj of a projectile of kinetic energy per nucleon T_proj.
  virtual double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const = 0;
};

using Pi0Gammas = SecondaryNeutrals;

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_NEUTRALS_SECONDARYNEUTRALS_H
