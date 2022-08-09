#include "XS4GCR/models/apTanNg83.h"

#include <cmath>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"

namespace XS4GCR {
namespace apTanNg83 {

double sigma_invariant(double sqrt_s, double X_r, double p_T) {
  using std::exp;
  using std::pow;

  double value = 0;
  if (X_r <= 1) {
    double A = 3.95 / cgs::GV;
    A *= exp(-2.76 * X_r);
    double B = 40.5 / pow2(cgs::GV);
    B *= exp(-3.21 * X_r) * pow(X_r, 2.13);
    double f = 2.10 * pow(1 - X_r, 7.80);
    if (X_r <= 0.5) f += 3.34 * exp(-17.6 * X_r);
    f *= cgs::mbarn * pow3(cgs::c) / pow2(cgs::GeV);
    value = f * exp(-(A * p_T + B * p_T * p_T));
  }
  return value;
}

double sigma_func(double eta, const double* params) {
  using std::cosh;
  using std::sqrt;
  using std::tanh;

  const double sqrt_s = params[0];
  const double s = sqrt_s * sqrt_s;
  const double E_ap = params[1];
  const double p_ap = params[2];

  const double gamma_cm = sqrt_s / 2 / cgs::protonMassC2;
  const double betagamma_cm = sqrt(s - 4 * pow2(cgs::protonMassC2)) / 2. / cgs::protonMassC2;
  const double E_ap_cm = gamma_cm * E_ap - betagamma_cm * p_ap * cgs::c * tanh(eta);
  double X_r = 2 * sqrt_s * E_ap_cm;
  X_r /= s - 8 * pow2(cgs::protonMassC2);
  double p_T = p_ap / cosh(eta);

  return (X_r > 1) ? 0 : sigma_invariant(sqrt_s, X_r, p_T) / pow2(cosh(eta));
}

double dsigma_dT(double T_p, double T_ap) {
  using std::sqrt;

  double result = 0;
  double error = 0;
  const double s = 2 * T_p * cgs::protonMassC2 + 4 * pow2(cgs::protonMassC2);
  const double sqrt_s = sqrt(s);
  const double p_ap = sqrt(T_ap * T_ap + 2 * cgs::protonMassC2 * T_ap) / cgs::c;
  if (sqrt_s > 4 * cgs::protonMassC2) {
    double params[3] = {sqrt_s, T_ap + cgs::protonMassC2, p_ap};
    result = GSL::gslQAGIUIntegration<double>([params](double eta) { return sigma_func(eta, params); }, 0., 1000);

    // gsl_function F;
    // F.function = &sigma_func;
    // F.params = &params;
    // gsl_integration_qagiu(&F, 0., 0., EPSREL, LIMIT, w, &result, &error);
    // gsl_integration_workspace_free(w);
  }
  double neutron_factor = 2.3;
  return neutron_factor * 2 * M_PI * p_ap * result / pow2(cgs::c);
}

}  // namespace apTanNg83
}  // namespace XS4GCR