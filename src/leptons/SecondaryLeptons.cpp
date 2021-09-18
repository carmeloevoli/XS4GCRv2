#include "XS4GCR/leptons/SecondaryLeptons.h"

#include <cmath>

#include "XS4GCR/cgs.h"

namespace XS4GCR {

double SecondaryLeptons::annihilationXsec(const PID& target, const double& T_positron) const {
  double Gamma = 1.0 + T_positron / cgs::electronMassC2;
  double logGamma = std::log(Gamma + std::sqrt(pow2(Gamma) - 1));
  double value = target.getZ() * M_PI * pow2(cgs::electronRadius) / (Gamma + 1.);
  value *= (pow2(Gamma) + 4 * Gamma + 1) / (pow2(Gamma) - 1) * logGamma - (Gamma + 3) / std::sqrt(pow2(Gamma) - 1);
  return value;
}

}  // namespace XS4GCR
