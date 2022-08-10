#include "XS4GCR/gammas/Kelner2006.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/models/gammaKelner06.h"

namespace XS4GCR {

void Kelner2006Gammas::print() const {
  LOGI << "using KELNER2006 gamma production model: ";
  LOGI << "Kelner et al., 2006, Physical Review D, vol.74, Issue 3, id.034018 ";
}

std::shared_ptr<Pi0Gammas> Kelner2006Gammas::clone() { return std::make_shared<Kelner2006Gammas>(*this); }

double Kelner2006Gammas::get(const PID& projectile, const TARGET& target, const double& T_proj,
                             const double& T_g) const {
  double sigma_pp = Kelner06::dsigma_dT(T_proj, T_g);
  if (projectile == H1 && target == TARGET::H) {
    return sigma_pp;
  } else {
    return 0;
  }
}

}  // namespace  XS4GCR