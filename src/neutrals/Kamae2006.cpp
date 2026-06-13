#include "XS4GCR/neutrals/Kamae2006.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/shared/secKamae06.h"

namespace XS4GCR {

void Kamae2006Neutrals::print() const {
  LOGI << "using KAMAE2006 neutral production model: ";
  LOGI << "Kamae et al., 2006, The Astrophysical Journal, Volume 647, Issue 1, pp. 692-708";
}

std::shared_ptr<SecondaryNeutrals> Kamae2006Neutrals::clone() { return std::make_shared<Kamae2006Neutrals>(*this); }

double Kamae2006Neutrals::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                          const double& x) const {
  if (x > 1.) return 0.;
  const double T_ph = x * T_proj;
  double value = 0;
  if (m_type == NeutralParticleType::GAMMA) {
    value = Kamae06::getCparamSigma(Kamae06::KGAMMA, T_proj, T_ph) * T_proj;
  } else if (m_type == NeutralParticleType::ALLNUS) {
    value = Kamae06::getCparamSigma(Kamae06::KNU, T_proj, T_ph) * T_proj;
  }
  if (projectile == H1 && target == TARGET::H) {
    return value;
  } else if (projectile == He4 && target == TARGET::H) {
    return std::pow(4., 2. / 3.) * value;
  } else if (projectile == H1 && target == TARGET::He) {
    return std::pow(4., 2. / 3.) * value;
  } else {
    return 0;
  }
}
}  // namespace  XS4GCR
