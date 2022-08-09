#include "XS4GCR/leptons/Kamae2006.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/models/secKamae06.h"

namespace XS4GCR {

// double HeFunction(double T_proton_GeV, double& T_lepton_GeV) {
//   double sigmaPlus = getCparamlibSigma(positron, T_proton_GeV, T_lepton_GeV);
//   double sigmaMinus = getCparamlibSigma(electron, T_proton_GeV, T_lepton_GeV);
//   double value = 1;
//   if (sigmaPlus > 0 && sigmaMinus > 0) {
//     value /= .25 * sigmaPlus / sigmaMinus + .25 * sigmaMinus / sigmaPlus;
//   }
//   return value;
// }

void Kamae2006SecLep::print() const {
  LOGI << "using KAMAE2006 lepton production model: ";
  LOGI << "\n";
}

std::shared_ptr<SecondaryLeptons> Kamae2006SecLep::clone() { return std::make_shared<Kamae2006SecLep>(*this); }

double Kamae2006SecLep::get(const PID& projectile, const TARGET& target, const double& T_proj,
                            const double& T_lepton) const {
  double sigma_pp = 0;
  if (m_lepton == positron)
    sigma_pp = Kamae06::getCparamSigma(Kamae06::KPOSITRON, T_proj, T_lepton);
  else
    sigma_pp = Kamae06::getCparamSigma(Kamae06::KELECTRON, T_proj, T_lepton);

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