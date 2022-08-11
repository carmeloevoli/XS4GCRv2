#include "XS4GCR/leptons/Kamae2006.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/models/secKamae06.h"

namespace XS4GCR {

void Kamae2006SecLep::print() const {
  LOGI << "using KAMAE2006 lepton production model: ";
  LOGI << "Kamae et al., 2006, The Astrophysical Journal, Volume 647, Issue 1";
}

std::shared_ptr<SecondaryLeptons> Kamae2006SecLep::clone() { return std::make_shared<Kamae2006SecLep>(*this); }

double Kamae2006SecLep::get(const PID& projectile, const TARGET& target, const double& T_proj,
                            const double& T_lepton) const {
  double sigma_pp = 0;
  if (projectile == H1 && target == TARGET::H) {
    if (m_lepton == positron)
      sigma_pp = Kamae06::getCparamSigma(Kamae06::KPOSITRON, T_proj, T_lepton);
    else
      sigma_pp = Kamae06::getCparamSigma(Kamae06::KELECTRON, T_proj, T_lepton);
  }
  return sigma_pp;
}

}  // namespace XS4GCR