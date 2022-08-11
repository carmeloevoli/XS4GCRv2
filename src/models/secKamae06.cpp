#include "XS4GCR/models/secKamae06.h"

#include <algorithm>
#include <stdexcept>

#include "XS4GCR/core/cgs.h"
#include "cparamlib.h"

namespace XS4GCR {
namespace Kamae06 {

double getCparamSigma(cparamParticles particle, double T_proton, double T_secondary) {
  double T_proton_GeV = T_proton / cgs::GeV;
  double E_secondary_GeV;
  PARAMSET parameters;
  PARTICLE_IDS par;
  if (particle == cparamParticles::KELECTRON) {
    par = ID_ELECTRON;
    elec_param(T_proton_GeV, &parameters);
    E_secondary_GeV = (T_secondary + cgs::electronMassC2) / cgs::GeV;
  } else if (particle == cparamParticles::KPOSITRON) {
    par = ID_POSITRON;
    posi_param(T_proton_GeV, &parameters);
    E_secondary_GeV = (T_secondary + cgs::electronMassC2) / cgs::GeV;
  } else if (particle == cparamParticles::KGAMMA) {
    par = ID_GAMMA;
    gamma_param(T_proton_GeV, &parameters);
    E_secondary_GeV = (T_secondary) / cgs::GeV;
  } else {
    throw std::runtime_error("error in particle type");
  }
  double dsigma_dlogE = sigma_incl_tot(par, E_secondary_GeV, std::min(T_proton_GeV, 1e5), &parameters);
  return dsigma_dlogE / E_secondary_GeV * (cgs::mbarn / cgs::GeV);
}

}  // namespace Kamae06
}  // namespace XS4GCR