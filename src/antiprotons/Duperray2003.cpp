#include "XS4GCR/antiprotons/Duperray2003.h"

#include <iostream>

#include "XS4GCR/models/apDuperray03.h"

namespace XS4GCR {

void Duperray2003SecAp::print() const {
  LOGI << "using DUPERRAY2003 antiproton model: ";
  LOGI << "Duperray et al., 2003, Physical Review D, vol.68, Issue 9, id.094017 ";
}

std::shared_ptr<SecondaryAntiprotons> Duperray2003SecAp::clone() { return std::make_shared<Duperray2003SecAp>(*this); }

double Duperray2003SecAp::get(const PID& particle, const TARGET& target, const double& T_n, const double& T_ap) const {
  if (particle == H1 && target == TARGET::H)
    return apDuperray03::dsigma_dT(T_n, T_ap, apDuperray03::PP);
  else if (particle == He4 || target == TARGET::He)
    return apDuperray03::dsigma_dT(T_n, T_ap, apDuperray03::PHE);
  else
    return 0;
}

}  // namespace XS4GCR