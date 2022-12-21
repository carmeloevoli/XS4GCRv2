// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/inelastic/Glauber.h"

#include <iostream>
#include <memory>

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

void GlauberTotalInel::print() const {
  LOGI << "using Glauber total inelastic model:";
  LOGI << "Grichine, 2009, Eur. Phys. J. C, 62, 399";
}

std::shared_ptr<TotalInelastic> GlauberTotalInel::clone() { return std::make_shared<GlauberTotalInel>(*this); }

double wilsonRmsRadius(const int& A) {
  double radius = 0;
  if (A > 26) {
    radius = 0.84 * pow(double(A), 1. / 3.) + 0.55;
  } else {
    std::vector<double> r = {0.0,   0.85, 2.095, 1.976, 1.671, 1.986, 2.57,  2.41,  2.23,
                             2.519, 2.45, 2.42,  2.471, 2.440, 2.58,  2.611, 2.730, 2.662,
                             2.727, 2.9,  3.040, 2.867, 2.969, 2.94,  3.075, 3.11,  3.06};
    radius = r.at(A);
  }
  return radius * cgs::fm;
}

inline double sigmappTot(double T) {
  auto plab = std::sqrt(T * T + 2. * cgs::protonMassC2 * T) / cgs::GeV;
  const double a = 20.52069;
  const double b = 0.09001;
  const double c = 30.59911;
  const double d = 0.31521;
  return (a * std::pow(plab, b) + c * std::pow(plab, -d)) * cgs::mbarn;
}

double GlauberTotalInel::get(const PID& projectile, const TARGET& target, const double& T_n) const {
  assert(projectile != PID(1, 1));
  auto A = projectile.getA();
  auto Z = projectile.getZ();
  auto piR2 = M_PI * pow2(wilsonRmsRadius(A));
  auto AsigmaTot = (double)A * sigmappTot(T_n);
  return piR2 * std::log(1. + AsigmaTot / piR2);
}

}  // namespace XS4GCR
