#include "XS4GCR/models/gammaKelner06.h"

#include <cmath>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/common.h"

namespace XS4GCR {
namespace Kelner06 {

double sigma_gamma(double E_proj, double E_gamma) {
  const double L = std::log(E_proj / cgs::TeV);  // defined in pag. 9
  const double x = E_gamma / E_proj;             // defined in pag. 9

  double B_gamma = 1.30 + 0.14 * L + 0.011 * L * L;            // Eq. 59
  double beta_gamma = 1. / (1.79 + 0.11 * L + 0.008 * L * L);  // Eq. 60
  double k_gamma = 1. / (0.801 + 0.049 * L + 0.014 * L * L);   // Eq. 61
  double x2beta = std::pow(x, beta_gamma);

  double F_1 = (1. - x2beta) / (1. + k_gamma * x2beta * (1. - x2beta));
  double F_2 = 4. * beta_gamma * x2beta / (1. - x2beta);
  double F_3 = 4. * k_gamma * beta_gamma * x2beta * (1. - 2. * x2beta);
  F_3 /= 1. + k_gamma * x2beta * (1. - x2beta);

  double F_gamma = B_gamma * std::log(x) / x * std::pow(F_1, 4);  // Eq. 58
  F_gamma *= 1. / log(x) - F_2 - F_3;

  return ppInelastic(E_proj) * F_gamma / E_proj;
}

double sigma_neutrinos(double E_proj, double E_nu) {
  const double L = std::log(E_proj / cgs::TeV);  // defined in pag. 9
  const double x = E_nu / E_proj;

  const double B_e = 1.0 / (69.5 + 2.65 * L + 0.3 * L * L);                                // Eq. 63
  const double beta_e = 1. / std::pow(0.201 + 0.062 * L + 0.00042 * L * L, 0.25);          // Eq. 64
  const double k_e = (0.279 + 0.141 * L + 0.0172 * L * L) / (0.3 + std::pow(2.3 + L, 2));  // Eq. 65

  const double y = x / 0.427;

  double F_e = 0, F_numu = 0;

  if (L > -3.) {
    F_e = B_e * std::pow(1.0 + k_e * std::pow(std::log(x), 2), 3) * std::pow(-std::log(x), 5);
    F_e /= x * (1.0 + 0.3 / std::pow(x, beta_e));  // Eq. 62
  }

  if (std::isnan(F_e)) throw std::runtime_error("F_e is NAN!");

  if (y < 1) {
    double B_prime = 1.75 + 0.204 * L + 0.010 * L * L;             // Eq. 67
    double beta_prime = 1. / (1.67 + 0.111 * L + 0.0038 * L * L);  // Eq. 68
    double k_prime = 1.07 - 0.086 * L + 0.002 * L * L;             // Eq. 69
    double y2beta = std::pow(y, beta_prime);
    double F_1 = 4. * beta_prime * y2beta / (1. - y2beta);
    double F_2 = 4. * k_prime * beta_prime * y2beta * (1. - 2. * y2beta);
    F_2 /= 1. + k_prime * y2beta * (1. - y2beta);
    F_numu = B_prime * std::log(y) / y * std::pow((1. - y2beta) / (1. + k_prime * y2beta * (1. - y2beta)), 4);
    F_numu *= 1. / std::log(y) - F_1 - F_2;  // Eq. 66
  }

  if (std::isnan(F_numu)) throw std::runtime_error("F_numu is NAN!");

  return ppInelastic(E_proj) * (F_numu + 2. * F_e) / E_proj;
}

}  // namespace Kelner06
}  // namespace XS4GCR