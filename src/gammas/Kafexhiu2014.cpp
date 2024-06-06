#include "XS4GCR/gammas/Kafexhiu2014.h"

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

void Kafexhiu2014Gammas::print() const {
  LOGI << "using KAFEXHIU2014 gamma production model: ";
  LOGI << "Kafexhiu et al., 2014, Phys. Rev. D 90, 123014";
}

std::shared_ptr<Pi0Gammas> Kafexhiu2014Gammas::clone() { return std::make_shared<Kafexhiu2014Gammas>(*this); }

double Kafexhiu2014Gammas::get(const PID& projectile, const TARGET& target, const double& T_proj,
                               const double& x) const {
  const double T_ph = x * T_proj;
  const double units = cgs::mbarn / cgs::GeV;
  double value = 0;
  if (projectile == H1 && target == TARGET::H && m_type == NeutralParticleType::GAMMA) {
    if (m_model == InteractionModel::GEANT4) {
      value = ppgam::dXSdEg_Geant4(T_proj / cgs::GeV, T_ph / cgs::GeV) * units;
    } else if (m_model == InteractionModel::PYTHIA8) {
      value = ppgam::dXSdEg_Pythia8(T_proj / cgs::GeV, T_ph / cgs::GeV) * units;
    } else if (m_model == InteractionModel::SIBYLL) {
      value = ppgam::dXSdEg_SIBYLL(T_proj / cgs::GeV, T_ph / cgs::GeV) * units;
    }
  }
  return T_proj * value;
}

}  // namespace  XS4GCR
