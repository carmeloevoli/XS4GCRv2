#ifndef INCLUDE_XS4GCR_NEUTRALS_KAFEXHIU2014_H
#define INCLUDE_XS4GCR_NEUTRALS_KAFEXHIU2014_H

#include "XS4GCR/neutrals/SecondaryNeutrals.h"

namespace XS4GCR {

enum class InteractionModel { GEANT4, PYTHIA8, SIBYLL };

class Kafexhiu2014Neutrals : public SecondaryNeutrals {
 public:
  Kafexhiu2014Neutrals(NeutralParticleType type, InteractionModel model) : m_type(type), m_model(model) {}

  void print() const override;

  std::shared_ptr<SecondaryNeutrals> clone() override;

  double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  NeutralParticleType m_type;
  InteractionModel m_model;
};

using Kafexhiu2014Gammas = Kafexhiu2014Neutrals;

}  // namespace XS4GCR

#endif
