#ifndef INCLUDE_XS4GCR_NEUTRALS_KAMAE2006_H
#define INCLUDE_XS4GCR_NEUTRALS_KAMAE2006_H

#include "XS4GCR/neutrals/SecondaryNeutrals.h"

namespace XS4GCR {

class Kamae2006Neutrals : public SecondaryNeutrals {
 public:
  Kamae2006Neutrals(NeutralParticleType type) : m_type(type) {}

  void print() const override;

  std::shared_ptr<SecondaryNeutrals> clone() override;

  double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  NeutralParticleType m_type;
};

using Kamae2006Gammas = Kamae2006Neutrals;

}  // namespace XS4GCR

#endif
