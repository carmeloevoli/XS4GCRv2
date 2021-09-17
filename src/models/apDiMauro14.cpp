#include "XS4GCR/models/apDiMauro14.h"

#include <algorithm>
#include <cmath>

#include "XS4GCR/cgs.h"
#include "XS4GCR/gsl.h"

namespace XS4GCR {
namespace apDiMauro14 {

double sigma_total(double s_GeV) {
  using std::log;
  using std::pow;

  const double Zt = 33.44;
  const double Mt = 2.06;
  const double Y1t = 13.53;
  const double Y2t = 6.38;
  const double eta1t = 0.324;
  const double eta2t = 0.324;
  const double mp = 0.938;
  const double pi_hbar_c__mbarn_2 = 1.22327;
  const double smt = pow2(2 * mp + Mt);
  const double Bt = pi_hbar_c__mbarn_2 / pow2(Mt);
  return Zt + Bt * pow2(log(s_GeV / smt)) + Y1t * pow(smt / s_GeV, eta1t) - Y2t * pow(smt / s_GeV, eta2t);
}

double sigma_elastic(double s_GeV) {
  using std::log;
  using std::pow;

  const double pi_hbar_c__mbarn_2 = 1.22327;
  const double mp = 0.938;
  const double Ze = 144.98;
  const double Me = 3.06;
  const double sme = pow2(2 * mp + Me);
  const double Be = pi_hbar_c__mbarn_2 / pow2(Me);
  const double Y1e = 2.64;
  const double Y2e = 137.27;
  const double eta1e = 1.57;
  const double eta2e = -4.65 * 1e-3;

  return Ze + Be * pow(log(s_GeV / sme), 2.) + Y1e * pow(sme / s_GeV, eta1e) - Y2e * pow(sme / s_GeV, eta2e);
}

double fitting_func(double X_r, double sqrt_s_GeV, double p_T_GeV) {
  using std::exp;
  using std::pow;

  const double C1 = 4.49878e+00;
  const double C2 = 3.40740e+00;
  const double C3 = 9.41840e-03;
  const double C4 = 4.44547e-01;
  const double C5 = 3.50178e+00;
  const double C6 = 6.22119e-02;
  const double C7 = -2.47658e-01;
  const double C8 = 2.57568e+00;

  double value = pow(1. - X_r, C1) * exp(-C2 * X_r);
  value *= C3 * pow(sqrt_s_GeV, C4) * exp(-C5 * p_T_GeV) + C6 * pow(sqrt_s_GeV, C7) * exp(-C8 * p_T_GeV * p_T_GeV);
  return value;
}

double sigma_invariant(double sqrt_s, double X_r, double p_T) {
  using std::max;
  using std::min;

  double value = 0;
  if (X_r <= 1) {
    const double sqrt_s_GeV = min(sqrt_s / cgs::GeV, 1e3);
    const double p_T_GeVc = min(p_T / cgs::GV, 1e3);
    const double s_GeV = sqrt_s_GeV * sqrt_s_GeV;
    double sigma_inelastic = sigma_total(s_GeV) - sigma_elastic(s_GeV);
    value = sigma_inelastic * fitting_func(X_r, sqrt_s_GeV, p_T_GeVc);
  }
  return max(value, 0.) * cgs::mbarn * pow3(cgs::c) / pow2(cgs::GeV);
}

double sigma_func(double eta, const double *params) {
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

  return (X_r > 1) ? 0 : sigma_invariant(sqrt_s, X_r, p_T) / pow2(cosh(eta));
}

double dsigma_dT(double T_p, double T_ap) {
  double result = 0;
  const double s = 2 * T_p * cgs::protonMassC2 + 4 * pow2(cgs::protonMassC2);
  const double sqrt_s = sqrt(s);
  const double p_ap = sqrt(T_ap * T_ap + 2 * cgs::protonMassC2 * T_ap) / cgs::c;
  if (sqrt_s > 4 * cgs::protonMassC2) {
    double params[3] = {sqrt_s, T_ap + cgs::protonMassC2, p_ap};
    result = GSL::gslQAGIUIntegration<double>([params](double eta) { return sigma_func(eta, params); }, 0., 1000);

    //     gsl_integration_workspace *w = gsl_integration_workspace_alloc(LIMIT);
    //     double params[3] = {sqrt_s, (T_ap + proton_mass_c2), p_ap};
    //     gsl_function F;
    //     F.function = &sigma_func;
    //     F.params = &params;
    //     gsl_integration_qagiu(&F, 0., 0., EPSREL, LIMIT, w, &result, &error);
    //     gsl_integration_workspace_free(w);
  }
  const double neutron_factor = 2.3;
  return neutron_factor * 2 * M_PI * p_ap * result / pow2(cgs::c);
}

}  // namespace apDiMauro14
}  // namespace XS4GCR
