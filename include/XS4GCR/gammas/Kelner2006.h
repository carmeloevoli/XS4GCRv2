#ifndef INCLUDE_XS4GCR_GAMMAS_KELNER2006_H
#define INCLUDE_XS4GCR_GAMMAS_KELNER2006_H

#include "XS4GCR/gammas/Pi0Gammas.h"

namespace XS4GCR {

class Kelner2006Gammas : public Pi0Gammas {
 public:
  Kelner2006Gammas(NeutralParticleType type) : m_type(type) {}

  void print() const override;

  std::shared_ptr<Pi0Gammas> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_ph) const override;

 protected:
  NeutralParticleType m_type;
};

}  // namespace XS4GCR

#endif