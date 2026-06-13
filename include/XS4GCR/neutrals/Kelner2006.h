#ifndef INCLUDE_XS4GCR_NEUTRALS_KELNER2006_H
#define INCLUDE_XS4GCR_NEUTRALS_KELNER2006_H

#include "XS4GCR/neutrals/SecondaryNeutrals.h"

namespace XS4GCR {

class Kelner2006Neutrals : public SecondaryNeutrals {
 public:
  Kelner2006Neutrals(NeutralParticleType type) : m_type(type) {}

  void print() const override;

  std::shared_ptr<SecondaryNeutrals> clone() override;

  double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  NeutralParticleType m_type;
};

using Kelner2006Gammas = Kelner2006Neutrals;

}  // namespace XS4GCR

#endif
