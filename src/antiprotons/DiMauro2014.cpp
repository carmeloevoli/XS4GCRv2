#include "XS4GCR/antiprotons/DiMauro2014.h"

#include <iostream>

#include "XS4GCR/models/apDiMauro14.h"

namespace XS4GCR {

void DiMauro2014SecAp::print() const {
  LOGI << "using DIMAURO2014 antiproton model: ";
  LOGI << "Di Mauro et al., 2014, Physical Review D, Volume 90, Issue 8, id .085017 ";
}

std::shared_ptr<SecondaryAntiprotons> DiMauro2014SecAp::clone() { return std::make_shared<DiMauro2014SecAp>(*this); }

double DiMauro2014SecAp::get(const PID& particle, const TARGET& target, const double& T_proj, const double& x) const {
  const double T_ap = x * T_proj;
  if (particle == H1 && target == TARGET::H)
    return T_proj * apDiMauro14::dsigma_dT(T_proj, T_ap);
  else
    return 0;
}

}  // namespace XS4GCR