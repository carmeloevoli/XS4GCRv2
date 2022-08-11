#include "XS4GCR/gammas/Kamae2006.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/models/secKamae06.h"

namespace XS4GCR {

void Kamae2006Gammas::print() const {
  LOGI << "using KAMAE2006 gamma production model: ";
  LOGI << "Kamae et al., 2006, The Astrophysical Journal, Volume 647, Issue 1, pp. 692-708";
}

std::shared_ptr<Pi0Gammas> Kamae2006Gammas::clone() { return std::make_shared<Kamae2006Gammas>(*this); }

double Kamae2006Gammas::get(const PID& projectile, const TARGET& target, const double& T_proj,
                            const double& T_ph) const {
  double sigma_pp = 0;
  if (projectile == H1 && target == TARGET::H) {
    if (m_type == NeutralParticleType::GAMMA) {
      sigma_pp = Kamae06::getCparamSigma(Kamae06::KGAMMA, T_proj, T_ph);
    } else if (m_type == NeutralParticleType::ALLNUS) {
      sigma_pp = Kamae06::getCparamSigma(Kamae06::KNU, T_proj, T_ph);
    }
  }
  return sigma_pp;
}

}  // namespace  XS4GCR
