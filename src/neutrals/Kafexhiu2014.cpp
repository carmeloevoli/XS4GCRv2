#include "XS4GCR/neutrals/Kafexhiu2014.h"

#include "LibppGam.h"
#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

void Kafexhiu2014Neutrals::print() const {
  LOGI << "using KAFEXHIU2014 gamma-only neutral production model: ";
  LOGI << "Kafexhiu et al., 2014, Phys. Rev. D 90, 123014";
}

std::shared_ptr<SecondaryNeutrals> Kafexhiu2014Neutrals::clone() { return std::make_shared<Kafexhiu2014Neutrals>(*this); }

double Kafexhiu2014Neutrals::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                            const double& x) const {
  const double T_ph = x * T_proj;
  const double units = cgs::mbarn / cgs::GeV;
  double value = 0;
  if (projectile == H1 && target == TARGET::H && m_type == NeutralParticleType::GAMMA) {
    if (m_model == InteractionModel::GEANT4) {
      value = dXSdEg_Geant4(T_proj / cgs::GeV, T_ph / cgs::GeV) * units;
    } else if (m_model == InteractionModel::PYTHIA8) {
      value = dXSdEg_Pythia8(T_proj / cgs::GeV, T_ph / cgs::GeV) * units;
    } else if (m_model == InteractionModel::SIBYLL) {
      value = dXSdEg_SIBYLL(T_proj / cgs::GeV, T_ph / cgs::GeV) * units;
    }
  }
  return T_proj * value;
}

}  // namespace  XS4GCR
