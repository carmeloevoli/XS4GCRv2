// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/inelastic/Letaw1983.h"

#include <memory>

#include "XS4GCR/cgs.h"

namespace XS4GCR {

void Letaw1983TotalInel::print() const {
  LOGI << "using Letaw1983 total inelastic model:";
  LOGI << "Letaw et al. 1983, ApJS, 51, 271";
}

std::shared_ptr<TotalInelastic> Letaw1983TotalInel::clone() { return std::make_shared<Letaw1983TotalInel>(*this); }

double Letaw1983TotalInel::get(const PID& projectile, const TARGET& target, const double& T_n) const {
  assert(projectile != PID(1, 1));
  double value = 45 * cgs::mbarn * pow(projectile.getA(), 0.7);
  value *= (1.0 + 0.016 * sin(5.3 - 2.63 * log(projectile.getA())));
  if (T_n < 5.0 * cgs::GeV) value *= 1.0 - 0.62 * exp(-T_n / 0.2 / cgs::GeV) * sin(1.58 / pow(T_n / cgs::GeV, 0.28));
  if (projectile.getZ() == 2) value *= 0.8;
  if (projectile.getZ() == 4) value *= 1.0 + 0.75 * exp(-T_n / 0.075 / cgs::GeV);
  return value;
}

}  // namespace XS4GCR
