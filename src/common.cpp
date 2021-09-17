#include "XS4GCR/common.h"

#include "XS4GCR/cgs.h"

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

}  // namespace XS4GCR
