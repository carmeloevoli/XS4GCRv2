
#include "XS4GCR/models/apDuperray03.h"

#include <algorithm>
#include <cmath>

#include "XS4GCR/cgs.h"
#include "XS4GCR/gsl.h"

namespace XS4GCR {
namespace apDuperray03 {

// Table II. Page 3
static double C[11] = {0, 0.16990, 10.28, 2.269, 3.707, 0.009205, 0.4812, 3.3600, 0.063940, -0.1824, 2.4850};

// Table V. Page 8
static double D[8] = {0., 3.4610, 4.340, 0.007855, 0.5121, 3.6620, 0.023070, 3.2540};

double sigma_inelastic(double A) {
  using std::log;
  using std::pow;
  using std::sin;

  double sigma_0 = 45 * pow(A, 0.7);
  sigma_0 *= 1 + 0.016 * sin(5.3 - 2.63 * log(A));
  return sigma_0;
}

double sigma_invariant_pp(double sqrt_s, double X_r, double p_T) {
  using std::exp;
  using std::log;
  using std::max;
  using std::pow;

  const double sqrt_s_GeV = sqrt_s / cgs::GeV;
  const double p_T_GeVc = p_T / cgs::GV;

  double value = sigma_inelastic(1.);
  value *= pow(1. - X_r, D[1]);
  value *= exp(-D[2] * X_r);
  value *= D[3] * pow(sqrt_s_GeV, D[4]) * exp(-D[5] * p_T_GeVc) + D[6] * exp(-D[7] * p_T_GeVc * p_T_GeVc);

  return max(value, 0.) * cgs::mbarn * pow3(cgs::c) / pow2(cgs::GeV);
}

double sigma_func_pp(double eta, const double *params) {
  using std::cosh;
  using std::sqrt;
  using std::tanh;

  const double sqrt_s = params[0];
  const double s = sqrt_s * sqrt_s;
  const double E_ap = params[1];
  const double p_ap = params[2];

  const double gamma_cm = sqrt_s / 2 / cgs::protonMassC2;
  const double betagamma_cm = sqrt(s - 4 * pow2(cgs::protonMassC2)) / 2 / cgs::protonMassC2;
  const double E_ap_cm = gamma_cm * E_ap - betagamma_cm * p_ap * cgs::c * tanh(eta);

  double X_r = 2 * sqrt_s * E_ap_cm;
  X_r /= s - 8 * pow2(cgs::protonMassC2);
  double p_T = p_ap / cosh(eta);

  return (X_r > 1) ? 0 : sigma_invariant_pp(sqrt_s, X_r, p_T) / pow2(cosh(eta));
}

double sigma_invariant_pHe(double sqrt_s, double X_r, double p_T) {
  using std::exp;
  using std::log;
  using std::max;
  using std::pow;

  const double sqrt_s_GeV = sqrt_s / cgs::GeV;
  const double p_T_GeVc = p_T / cgs::GV;
  const double A_He = 4.;

  double value = sigma_inelastic(A_He);
  value *= pow(A_He, C[1] * log(sqrt_s_GeV / C[2]) * p_T_GeVc);
  value *= pow(1. - X_r, C[3] * log(sqrt_s_GeV));
  value *= exp(-C[4] * X_r);
  value *= C[5] * pow(sqrt_s_GeV, C[6]) * exp(-C[7] * p_T_GeVc) +
           C[8] * pow(sqrt_s_GeV, C[9]) * exp(-C[10] * p_T_GeVc * p_T_GeVc);

  return max(value, 0.) * cgs::mbarn * pow3(cgs::c) / pow2(cgs::GeV);
}

double sigma_func_pHe(double eta, const double *params) {
  using std::cosh;
  using std::sqrt;
  using std::tanh;

  const double sqrt_s = params[0];
  const double s = sqrt_s * sqrt_s;
  const double E_ap = params[1];
  const double p_ap = params[2];

  const double gamma_cm = sqrt_s / 2 / cgs::protonMassC2;
  const double betagamma_cm = sqrt(s - 4 * pow2(cgs::protonMassC2)) / 2 / cgs::protonMassC2;
  const double E_ap_cm = gamma_cm * E_ap - betagamma_cm * p_ap * cgs::c * tanh(eta);

  double X_r = 2 * sqrt_s * E_ap_cm;
  X_r /= s - 8 * pow2(cgs::protonMassC2);
  double p_T = p_ap / cosh(eta);

  return (X_r > 1) ? 0 : sigma_invariant_pHe(sqrt_s, X_r, p_T) / pow2(cosh(eta));
}

double dsigma_dT(double T_n, double T_ap, Channel channel) {
  using std::sqrt;

  double result = 0;
  double error = 0;
  const double s = 2 * T_n * cgs::protonMassC2 + 4 * pow2(cgs::protonMassC2);
  const double sqrt_s = sqrt(s);
  const double p_ap = sqrt(pow2(T_ap) + 2 * cgs::protonMassC2 * T_ap) / cgs::c;
  if (sqrt_s > 4 * cgs::protonMassC2) {
    double params[3] = {sqrt_s, T_ap + cgs::protonMassC2, p_ap};
    if (channel == PP)
      result = GSL::gslQAGIUIntegration<double>([params](double eta) { return sigma_func_pp(eta, params); }, 0., 1000);
    else
      result = GSL::gslQAGIUIntegration<double>([params](double eta) { return sigma_func_pHe(eta, params); }, 0., 1000);
  }
  double neutron_factor = 2.3;
  return neutron_factor * 2 * M_PI * p_ap * result / pow2(cgs::c);
}

}  // namespace apDuperray03
}  // namespace XS4GCR