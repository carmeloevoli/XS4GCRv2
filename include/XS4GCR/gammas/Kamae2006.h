#ifndef INCLUDE_XS4GCR_GAMMAS_KAMAE2006_H
#define INCLUDE_XS4GCR_GAMMAS_KAMAE2006_H

#include "XS4GCR/gammas/Pi0Gammas.h"

namespace XS4GCR {

class Kamae2006Gammas : public Pi0Gammas {
 public:
  Kamae2006Gammas(NeutralParticleType type) : m_type(type) {}

  void print() const override;

  std::shared_ptr<Pi0Gammas> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  NeutralParticleType m_type;
};

}  // namespace XS4GCR

#endif