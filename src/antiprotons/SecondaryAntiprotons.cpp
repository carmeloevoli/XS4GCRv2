#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"

#include <cmath>

#include "XS4GCR/cgs.h"

namespace XS4GCR {

double SecondaryAntiprotons::getTotalInelastic(const TARGET& target, const double& T_ap) {
  using std::exp;
  using std::log;
  using std::pow;
  using std::sin;

  double value = 0;
  if (target == TARGET::H) {
    double E_GeV = (T_ap + cgs::protonMassC2) / cgs::GeV;
    double R_GV = std::sqrt(pow2(E_GeV) - pow2(cgs::protonMassC2));
    value = a[0] + a[1] * log(R_GV) + a[2] * pow2(log(R_GV)) + a[3] * pow(R_GV, -alpha);
  } else {  // duperray2003
    double sigma_0 = 45. * pow(4, 0.7) * (1. + 0.016 * sin(5.3 - 2.63 * log(4.)));
    double E_inc = T_ap / cgs::MeV;
    value = sigma_0 * (1. - 0.62 * exp(-E_inc / 200) * sin(10.9 * pow(E_inc, -0.28)));
  }
  return value * cgs::mbarn;
}

double SecondaryAntiprotons::getAnnihilatingInelastic(const TARGET& target, const double& T_ap) {
  double E_GeV = (T_ap + cgs::protonMassC2) / cgs::GeV;
  double R_GV = std::sqrt(pow2(E_GeV) - pow2(cgs::protonMassC2));
  double value = b[0] + b[1] * log(R_GV) + b[2] * pow2(log(R_GV)) + b[3] * pow(R_GV, -alpha);
  if (target == TARGET::He) {
    value *= getTotalInelastic(target, T_ap) / getTotalInelastic(TARGET::H, T_ap);
  }
  return value * cgs::mbarn;
}

double SecondaryAntiprotons::getNonAnnihilatingInelastic(const TARGET& target, const double& T_ap) {
  return getTotalInelastic(target, T_ap) - getAnnihilatingInelastic(target, T_ap);
}

}  // namespace XS4GCR
