#include "XS4GCR/gammas/Kafexhiu2014.h"

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

void Kafexhiu2014Gammas::print() const {
  LOGI << "using KAFEXHIU2014 gamma production model: ";
  LOGI << "Kafexhiu et al., 2014, Phys. Rev. D 90, 123014";
}

std::shared_ptr<Pi0Gammas> Kafexhiu2014Gammas::clone() { return std::make_shared<Kafexhiu2014Gammas>(*this); }

double Kafexhiu2014Gammas::get(const PID& projectile, const TARGET& target, const double& T_proj,
                               const double& T_g) const {
  double sigma_pp = Kafexhiu14::dsigma_dE_gamma(T_proj, T_g, m_model);
  if (projectile == H1 && target == TARGET::H) {
    return sigma_pp;
  } else {
    return 0;
  }
}

}  // namespace  XS4GCR
