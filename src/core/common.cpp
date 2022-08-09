#include "XS4GCR/core/common.h"

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

double T2beta(const double& T) {
  if (T < 0.0) throw std::invalid_argument("T must be positive");
  double beta = std::sqrt(T * (T + 2. * cgs::protonMassC2));
  beta /= T + cgs::protonMassC2;
  return beta;
}

double T2gamma(const double& T) {
  if (T < 0.0) throw std::invalid_argument("T must be positive");
  double gamma = T + cgs::protonMassC2;
  gamma /= cgs::protonMassC2;
  return gamma;
}

double T2pc(const double& T, const PID& pid) {
  if (T < 0.0) throw std::invalid_argument("T must be positive");
  double pc = std::sqrt(T * (T + 2. * cgs::protonMassC2));
  pc *= (double)pid.getA();
  return pc;
}

double ppInelastic(double T) {  // TODO find reference
  constexpr double T_threshold = 0.2797 * cgs::GeV;
  const double x = T / T_threshold;
  double value = 0;
  if (x > 1) {
    value = 30.7 - 0.96 * log(x) + 0.18 * pow2(log(x));
    value *= pow3(1 - pow(x, -1.9));
  }
  return value * cgs::mbarn;
}

}  // namespace XS4GCR
