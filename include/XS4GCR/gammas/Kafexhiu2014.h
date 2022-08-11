#ifndef INCLUDE_XS4GCR_GAMMAS_KAFEXHIU2014_H
#define INCLUDE_XS4GCR_GAMMAS_KAFEXHIU2014_H

#include "XS4GCR/gammas/Pi0Gammas.h"
#include "XS4GCR/models/gammaKafexhiu14.h"

namespace XS4GCR {

class Kafexhiu2014Gammas : public Pi0Gammas {
 public:
  Kafexhiu2014Gammas(Kafexhiu14::InteractionModel model) : m_model(model) {}

  void print() const override;

  std::shared_ptr<Pi0Gammas> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_g) const override;

 private:
  Kafexhiu14::InteractionModel m_model;
};

}  // namespace XS4GCR

#endif