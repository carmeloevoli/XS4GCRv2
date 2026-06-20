#include "XS4GCR/inelastic/NucleonNucleonXsc.h"

#include <cmath>

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {
namespace NN {

// Total nucleon-nucleon cross sections from J. W. Norbury, NASA/TP-2008-215116 (2008).
// All internal arithmetic follows the paper: kinetic energy T and lab momentum p in MeV,
// Mandelstam s in GeV^2, cross sections in mb. The three energy regimes (low: HZETRN fit;
// intermediate: dedicated fit; high: PDG form) are stitched together exactly as in eqs.
// (14)-(16) for pp and (18)-(20) for np; the joins are slightly discontinuous, as noted
// by Norbury.
namespace {

const double mN_MeV = cgs::protonMassC2 / cgs::MeV;  // nucleon mass (~938 MeV)

double plab_MeV(double T_MeV) { return std::sqrt(T_MeV * (T_MeV + 2. * mN_MeV)); }

// PDG high-energy parametrisation, eq. (10): sigma = Z + B ln^2(s/s0) + Y1 (s1/s)^eta1 - Y2 (s1/s)^eta2.
double sigmaPDG(double T_MeV, double Z, double Y1, double Y2) {
  const double B = 0.308;            // mb
  const double s0 = 5.38 * 5.38;     // GeV^2
  const double s1 = 1.0;             // GeV^2
  const double eta1 = 0.458;
  const double eta2 = 0.545;
  double pl = plab_MeV(T_MeV);
  double Elab = std::sqrt(pl * pl + mN_MeV * mN_MeV);
  double s = 2. * mN_MeV * (mN_MeV + Elab) * 1e-6;  // MeV^2 -> GeV^2
  double l = std::log(s / s0);
  return Z + B * l * l + Y1 * std::pow(s1 / s, eta1) - Y2 * std::pow(s1 / s, eta2);
}

// HZETRN low-energy pp fit, eqs. (6)-(7), constants from Table 1 (MeV units).
double sigmaHZETRN_pp(double T_MeV) {
  if (T_MeV >= 25.)
    return (1. + 5. / T_MeV) *
           (40. + 109. * std::cos(0.199 * std::sqrt(T_MeV)) * std::exp(-0.451 * std::pow(T_MeV - 25., 0.258)));
  return std::exp(6.51 * std::exp(-std::pow(T_MeV / 134., 0.7)));
}

// HZETRN low-energy np fit, eqs. (8)-(9), constants from Table 1 (MeV units).
double sigmaHZETRN_np(double T_MeV) {
  if (T_MeV >= 0.1) return 38. + 12500. * std::exp(-1.187 * std::pow(T_MeV - 0.1, 0.35));
  return 26000. * std::exp(-std::pow(T_MeV / 0.282, 0.3));
}

}  // namespace

double ppTotalXsc(const double& T) {
  double T_MeV = T / cgs::MeV;
  double pl = plab_MeV(T_MeV);
  double sigma_mb;
  if (pl < 1800.)
    sigma_mb = sigmaHZETRN_pp(T_MeV);  // eq. (14)
  else if (pl <= 4700.)
    sigma_mb = 158.547 / std::pow(pl, 0.16);  // eq. (13)/(15): gamma = 52.5 mb GeV^0.16
  else
    sigma_mb = sigmaPDG(T_MeV, 35.45, 42.53, 33.34);  // eq. (16), Table 2
  return sigma_mb * cgs::mbarn;
}

double pnTotalXsc(const double& T) {
  double T_MeV = T / cgs::MeV;
  double pl = plab_MeV(T_MeV);
  double sigma_mb;
  if (pl < 500.)
    sigma_mb = sigmaHZETRN_np(T_MeV);  // eq. (18)
  else if (pl <= 2000.)
    sigma_mb = 40. + 10. * std::cos(0.00369 * pl - 0.943) * std::exp(-0.00895741 * std::pow(pl, 0.8) + 2.);  // eq. (17)/(19)
  else
    sigma_mb = sigmaPDG(T_MeV, 35.80, 40.15, 30.00);  // eq. (20), Table 2
  return sigma_mb * cgs::mbarn;
}

}  // namespace NN
}  // namespace XS4GCR
