#ifndef INCLUDE_XS4GCR_GAMMAS_KAFEXHIU2014_H
#define INCLUDE_XS4GCR_GAMMAS_KAFEXHIU2014_H

#include "XS4GCR/gammas/Pi0Gammas.h"
#include "XS4GCR/models/gammaKafexhiu14.h"

namespace XS4GCR {

enum class InteractionModel { GEANT4, PYTHIA8, SIBYLL };

class Kafexhiu2014Gammas : public Pi0Gammas {
 public:
  Kafexhiu2014Gammas(NeutralParticleType type, InteractionModel model) : m_type(type), m_model(model) {}

  void print() const override;

  std::shared_ptr<Pi0Gammas> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  NeutralParticleType m_type;
  InteractionModel m_model;
};

}  // namespace XS4GCR

#endif