#include "XS4GCR/leptons/Kamae2006.h"

#include "XS4GCR/cgs.h"
#include "cparamlib.h"

namespace XS4GCR {

double getCparamlibSigma(PID lepton, double T_proton_GeV, double T_lepton_GeV) {
  PARAMSET parameters;
  PARTICLE_IDS par;
  if (lepton == electron) {
    par = ID_ELECTRON;
    elec_param(T_proton_GeV, &parameters);
  } else if (lepton == positron) {
    par = ID_POSITRON;
    posi_param(T_proton_GeV, &parameters);
  } else {
    throw std::runtime_error("particle is not a lepton");
  }
  double E_lepton_GeV = T_lepton_GeV + cgs::electronMassC2 / cgs::GeV;
  double dsigma_dlogT = sigma_incl_tot(par, E_lepton_GeV, std::min(T_proton_GeV, 1e5), &parameters);
  return dsigma_dlogT / T_lepton_GeV * (cgs::mbarn / cgs::GeV);
}

double HeFunction(double T_proton_GeV, double& T_lepton_GeV) {
  double sigmaPlus = getCparamlibSigma(positron, T_proton_GeV, T_lepton_GeV);
  double sigmaMinus = getCparamlibSigma(electron, T_proton_GeV, T_lepton_GeV);
  double value = 1;
  if (sigmaPlus > 0 && sigmaMinus > 0) {
    value /= .25 * sigmaPlus / sigmaMinus + .25 * sigmaMinus / sigmaPlus;
  }
  return value;
}

void Kamae2006SecLep::print() const {
  LOGI << "using KAMAE2006 lepton production model: ";
  LOGI << "\n";
}

std::shared_ptr<SecondaryLeptons> Kamae2006SecLep::clone() { return std::make_shared<Kamae2006SecLep>(*this); }

double Kamae2006SecLep::get(const PID& projectile, const TARGET& target, const double& T_proj,
                            const double& T_lepton) const {
  double sigma_pp = getCparamlibSigma(m_lepton, T_proj / cgs::GeV, T_lepton / cgs::GeV);
  if (projectile == H1 && target == TARGET::H) {
    return sigma_pp;
    //   } else if (projectile == He4 && target == TARGET::He) { // TODO what is that?
    //     return sigma_pp * HeHe_correction * ((lepton.is_electron()) ? He_function(T_n, T_lepton) : 1);
    //   } else {
    //     return sigma_pp * He_correction * ((lepton.is_electron()) ? He_function(T_n, T_lepton) : 1);
  } else {
    return 0;
  }
}

}  // namespace XS4GCR