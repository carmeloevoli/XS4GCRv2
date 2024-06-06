#include "XS4GCR/antiprotons/TanNg1983.h"

#include "XS4GCR/models/apTanNg83.h"

namespace XS4GCR {

void TanNg1983SecAp ::print() const {
  LOGI << "using TANNG1983 antiproton model: ";
  LOGI << "Tan and Ng, 1983, Journal of Physics G: Nuclear and Particle Physics, Volume 9, Issue 2, pp. 227-242";
}

std::shared_ptr<SecondaryAntiprotons> TanNg1983SecAp::clone() { return std::make_shared<TanNg1983SecAp>(*this); }

double TanNg1983SecAp::get(const PID& particle, const TARGET& target, const double& T_proj, const double& x) const {
  const auto T_ap = x * T_proj;
  if (particle == H1 && target == TARGET::H)
    return T_proj * apTanNg83::dsigma_dT(T_proj, T_ap);
  else
    return 0;
}

}  // namespace XS4GCR