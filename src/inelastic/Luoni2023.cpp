#include "XS4GCR/inelastic/Luoni2023.h"

#include <cassert>
#include <cmath>
#include <vector>

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {

// Tripathi et al. (1999) light-system total reaction cross section with the parameter
// optimisation proposed by Luoni et al. (2023), New J. Phys. 25 123024.
//
// Scope note. XS4GCR only ever evaluates this model for a cosmic-ray nucleus impinging
// on a hydrogen or helium ISM target, i.e. the lighter partner is always 1H or 4He.
// Following the Tripathi convention the lightest nucleus plays the role of projectile:
//   - H target  -> proton projectile. Luoni2023 proposes no change for proton
//                  projectiles, so this channel reduces to the original Tripathi99
//                  parametrisation (with Xm = 1, as recommended in the paper / ref. [7]).
//   - He target -> 4He projectile. The relevant update is the 4He-projectile
//                  optimisation of table 6, applied here.
// Two further global recommendations from the paper are also implemented:
//   - alpha = 5 for the neutron-excess multiplier when the heavy target has Z > 54
//     (default alpha = 0.91);
//   - Xm = 1 for every projectile but neutrons.
namespace {

double get_wilson_rms_radius(const int& A) {
  double factor = std::sqrt(5. / 3.) * cgs::fm;

  double radius;
  if (A > 26) {
    radius = factor * (0.84 * pow(double(A), 1. / 3.) + 0.55);
  } else {
    std::vector<double> r = {0.0,   0.85, 2.095, 1.976, 1.671, 1.986, 2.57,  2.41,  2.23,
                             2.519, 2.45, 2.42,  2.471, 2.440, 2.58,  2.611, 2.730, 2.662,
                             2.727, 2.9,  3.040, 2.867, 2.969, 2.94,  3.075, 3.11,  3.06};
    radius = factor * r.at(A);
  }
  return radius;
}

// Optimised Tripathi99 parameters for 4He + (Z_other) systems, table 6 of Luoni et al.
// (2023). useEq11 selects the steeper/earlier Coulomb-barrier form (their eq. 11) over
// the original eq. 4. A_cos is the coefficient of the cosine term in C_E (the "A"
// parameter, default 0.292). Returns false when the system is not listed, in which case
// the original Tripathi99 He parameters are used.
struct He4Params {
  double R_c;
  double T_1;
  double D0;
  double G;
  double A_cos;
  bool useEq11;
};

bool get_He4_optimised(int Z_other, He4Params& p) {
  switch (Z_other) {
    case 3:  p = {1.0,  40.,  2.95, 75.,  0.292, false}; return true;  // 7Li
    case 4:  p = {7.0,  50.,  2.2,  50.,  1.4,   true};  return true;  // 9Be
    case 6:  p = {3.5,  50.,  2.2,  50.,  0.7,   true};  return true;  // 12C
    case 8:  p = {1.0,  50.,  2.4,  50.,  0.292, true};  return true;  // 16O
    case 10: p = {1.0,  40.,  3.0,  75.,  0.292, false}; return true;  // 20Ne
    case 13: p = {1.0,  20.,  2.5,  300., 0.292, false}; return true;  // 27Al
    case 14: p = {1.0,  50.,  2.4,  50.,  0.292, true};  return true;  // 28Si
    case 20: p = {1.0,  100., 2.77, 75.,  0.292, false}; return true;  // 40Ca
    case 22: p = {1.0,  40.,  3.0,  300., 0.292, false}; return true;  // 48Ti
    case 23: p = {1.2,  40.,  3.0,  300., 0.292, false}; return true;  // 51V
    case 24: p = {1.2,  40.,  3.0,  300., 0.292, false}; return true;  // 52Cr
    case 26: p = {1.2,  40.,  3.0,  300., 0.292, false}; return true;  // 56Fe
    case 27: p = {1.1,  40.,  3.0,  300., 0.292, false}; return true;  // 59Co
    case 28: p = {1.2,  80.,  3.0,  300., 0.292, false}; return true;  // Ni
    case 29: p = {1.2,  80.,  3.2,  300., 0.292, false}; return true;  // 64Cu
    case 30: p = {1.1,  80.,  3.2,  300., 0.292, false}; return true;  // 65Zn
    case 40:                                                           // 91Zr
    case 41:                                                           // 93Nb
    case 42: p = {1.1,  80.,  3.4,  300., 0.292, false}; return true;  // 96Mo
    case 47: p = {1.0,  80.,  3.4,  300., 0.292, false}; return true;  // 108Ag
    case 50: p = {1.1,  80.,  3.4,  300., 0.292, false}; return true;  // Sn
    case 73: p = {1.1,  80.,  3.4,  300., 0.292, false}; return true;  // 181Ta
    case 79: p = {1.0,  80.,  3.4,  300., 0.292, false}; return true;  // 197Au
    case 82:                                                           // 207Pb
    case 83:                                                           // 209Bi
    case 90: p = {1.1,  75.,  3.7,  300., 0.292, false}; return true;  // 232Th
    case 93:                                                           // 237Np
    case 94: p = {1.15, 75.,  3.7,  300., 0.292, false}; return true;  // 239Pu
    default: return false;
  }
}

double inelastic_sigma(int A_p, int Z_p, int A_t, int Z_t, double T_n) {
  assert(A_p > 0 && A_t > 0);
  const double r_0 = 1.1 * cgs::fm;
  double f_A = pow2(A_p) + pow2(A_t) + 2 * A_p * A_t * (1. + T_n / cgs::protonMassC2);
  double E_cm_mev = cgs::protonMassC2 * std::sqrt(f_A);
  E_cm_mev /= cgs::MeV;
  double Tn_mev = T_n / cgs::MeV;

  if (E_cm_mev <= (0.8 + 0.04 * Z_t) * A_p) {
    return 0.;
  }

  double r_rms_p = get_wilson_rms_radius(A_p);
  double r_rms_t = get_wilson_rms_radius(A_t);
  double r_p = 1.29 * r_rms_p;
  double r_t = 1.29 * r_rms_t;
  double Radius = (r_p + r_t) / cgs::fm;
  Radius += 1.2 * (pow(A_p, 1. / 3.) + pow(A_t, 1. / 3.)) / pow(E_cm_mev, 1. / 3.);
  double B = 1.44 * Z_p * Z_t / Radius;

  const bool p_is_neutron = (A_p == 1 && Z_p == 0);
  const bool t_is_neutron = (A_t == 1 && Z_t == 0);
  const bool p_is_He4 = (A_p == 4 && Z_p == 2);
  const bool t_is_He4 = (A_t == 4 && Z_t == 2);

  double D = 2.05;
  double T_1 = 40.;
  double G = 75.;
  double A_cos = 0.292;  // coefficient of the cosine term in C_E (Luoni "A" parameter)
  double R_c = 1.0;
  bool he4_branch = false;

  // The proton / neutron / deuteron / 3He branches come first, so that the lightest
  // nucleus plays the projectile role (e.g. p + 4He is a proton projectile, not a 4He one).
  if ((A_t == 1 && Z_t == 1) || (A_p == 1 && Z_p == 1)) {
    T_1 = 23.0;
    D = 1.85 + 0.16 / (1. + exp((500. - Tn_mev) / 200.));
  } else if (t_is_neutron || p_is_neutron) {
    T_1 = 18.0;
    D = 1.85 + 0.16 / (1. + exp((500. - Tn_mev) / 200.));
  } else if ((A_t == 2 && Z_t == 1) || (A_p == 2 && Z_p == 1)) {
    T_1 = 23.0;
    D = 1.65 + 0.1 / (1. + exp((500. - Tn_mev) / 200.));
  } else if ((A_t == 3 && Z_t == 2) || (A_p == 3 && Z_p == 2)) {
    T_1 = 40.0;
    D = 1.55;
  } else if (p_is_He4 || t_is_He4) {
    he4_branch = true;
    int Z_other = p_is_He4 ? Z_t : Z_p;
    int A_other = p_is_He4 ? A_t : A_p;  // mass number of the (heavy) target in the D formula
    He4Params hp;
    if (get_He4_optimised(Z_other, hp)) {
      T_1 = hp.T_1;
      G = hp.G;
      A_cos = hp.A_cos;
      R_c = hp.R_c;
      double barrier = hp.useEq11 ? 0.3 / (1. + exp((120. - Tn_mev) / G))   // Luoni eq. (11)
                                  : 0.8 / (1. + exp((250. - Tn_mev) / G));  // Tripathi eq. (4)
      D = hp.D0 - 8e-3 * A_other + 1.8e-5 * pow2(A_other) - barrier;
    } else {
      // Systems not listed in table 6 keep the original Tripathi99 He parameters.
      if (Z_other == 4) {
        T_1 = 25.0;
        G = 300.0;
      } else if (Z_other == 7) {
        T_1 = 40.0;
        G = 500.0;
      } else if (Z_other == 13) {
        T_1 = 25.0;
        G = 300.0;
      } else if (Z_other == 26) {
        T_1 = 40.0;
        G = 300.0;
      } else if (Z_other == 2) {  // 4He + 4He
        T_1 = 40.0;
        G = 300.0;
      } else {
        T_1 = 40.0;
        G = 75.0;
      }
      D = 2.77 - 8e-3 * A_other + 1.8e-5 * pow2(A_other) - 0.8 / (1. + exp((250. - Tn_mev) / G));
    }
  }

  double C_E = D * (1 - exp(-Tn_mev / T_1));
  C_E -= A_cos * exp(-Tn_mev / 792.) * std::cos(0.229 * pow(Tn_mev, 0.453));

  double S = pow(A_p, 1. / 3.) * pow(A_t, 1. / 3.);
  S /= pow(A_p, 1. / 3.) + pow(A_t, 1. / 3.);

  // Neutron-excess multiplier: alpha = 5 for very heavy targets (Z > 54), 0.91 otherwise.
  int Z_heavy = (A_t >= A_p) ? Z_t : Z_p;
  double alpha = (Z_heavy > 54) ? 5.0 : 0.91;

  double delta_E = 1.85 * S;
  delta_E += 0.16 * S / pow(E_cm_mev, 1. / 3.);
  delta_E -= C_E;
  if (A_t >= A_p) {
    delta_E += alpha * (A_t - 2. * Z_t) * Z_p / A_t / A_p;
  } else {
    delta_E += alpha * (A_p - 2. * Z_p) * Z_t / A_t / A_p;
  }

  // Xm = 1 for every projectile but neutrons (Luoni 2023, sec. 2.2 / ref. [7]).
  double X_m = 1.0;
  if (p_is_neutron || t_is_neutron) {
    double X_1 = (A_t >= A_p) ? (2.83 - 3.1e-2 * A_t + 1.7e-4 * A_t * A_t)
                              : (2.83 - 3.1e-2 * A_p + 1.7e-4 * A_p * A_p);
    double S_L = 1.2 + 1.6 * (1 - exp(-Tn_mev / 15));
    X_m = 1 - X_1 * exp(-Tn_mev / X_1 / S_L);
  }

  // Coulomb multiplier R_c. The 4He systems set it from table 6 above; the remaining
  // light systems use the original Tripathi99 values.
  if (!he4_branch) {
    if (A_p == 1 && Z_p == 1) {
      if (A_t == 2 && Z_t == 1)
        R_c = 13.5;
      else if (A_t == 3 && Z_t == 2)
        R_c = 21.0;
      else if (A_t == 4 && Z_t == 2)
        R_c = 27.0;
      else if (Z_t == 3)
        R_c = 2.2;
    } else if (A_t == 1 && Z_t == 1) {
      if (A_p == 2 && Z_p == 1)
        R_c = 13.5;
      else if (A_p == 3 && Z_p == 2)
        R_c = 21.0;
      else if (A_p == 4 && Z_p == 2)
        R_c = 27.0;
      else if (Z_p == 3)
        R_c = 2.2;
    } else if (A_p == 2 && Z_p == 1) {
      if (A_t == 2 && Z_t == 1)
        R_c = 13.5;
      else if (A_t == 4 && Z_t == 2)
        R_c = 13.5;
      else if (A_t == 12 && Z_t == 6)
        R_c = 6.0;
    } else if (A_t == 2 && Z_t == 1) {
      if (A_p == 2 && Z_p == 1)
        R_c = 13.5;
      else if (A_p == 4 && Z_p == 2)
        R_c = 13.5;
      else if (A_p == 12 && Z_p == 6)
        R_c = 6.0;
    }
  }

  double value = M_PI * pow2(r_0);
  value *= pow2(pow(A_p, 1. / 3.) + pow(A_t, 1. / 3.) + delta_E);
  value *= (1. - R_c * B / E_cm_mev) * X_m;
  return std::max(0., value);
}

}  // namespace

void Luoni2023TotalInel::print() const {
  LOGI << "using Luoni+2023 (optimised Tripathi+99) total inelastic model:";
  LOGI << " ";
}

std::shared_ptr<TotalInelastic> Luoni2023TotalInel::clone() { return std::make_shared<Luoni2023TotalInel>(*this); }

double Luoni2023TotalInel::getTotal(const PID& projectile, const TARGET& target, const double& T_n) const {
  assert(projectile != PID(1, 1));
  int A_p = (target == TARGET::H) ? 1 : 4;
  int Z_p = (target == TARGET::H) ? 1 : 2;
  int A_t = projectile.getA();
  int Z_t = projectile.getZ();

  return inelastic_sigma(A_p, Z_p, A_t, Z_t, T_n);
}

}  // namespace XS4GCR
