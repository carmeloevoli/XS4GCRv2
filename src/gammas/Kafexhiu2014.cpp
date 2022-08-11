#include "XS4GCR/gammas/Kafexhiu2014.h"

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

void Kafexhiu2014Gammas::print() const {
  LOGI << "using KAFEXHIU2014 gamma production model: ";
  LOGI << "Kafexhiu et al., 2014, Phys. Rev. D 90, 123014";
}

std::shared_ptr<Pi0Gammas> Kafexhiu2014Gammas::clone() { return std::make_shared<Kafexhiu2014Gammas>(*this); }

double Kafexhiu2014Gammas::get(const PID& projectile, const TARGET& target, const double& T_proj,
                               const double& T_ph) const {
  double sigma_pp = 0;
  if (projectile == H1 && target == TARGET::H && m_type == NeutralParticleType::GAMMA) {
    if (m_model == InteractionModel::GEANT4) {
      sigma_pp = ppgam::dXSdEg_Geant4(T_proj / cgs::GeV, T_ph / cgs::GeV);
    } else if (m_model == InteractionModel::PYTHIA8) {
      sigma_pp = ppgam::dXSdEg_Pythia8(T_proj / cgs::GeV, T_ph / cgs::GeV);
    } else if (m_model == InteractionModel::SIBYLL) {
      sigma_pp = ppgam::dXSdEg_SIBYLL(T_proj / cgs::GeV, T_ph / cgs::GeV);
    }
  }
  return sigma_pp * cgs::mbarn / cgs::GeV;
}

}  // namespace  XS4GCR
