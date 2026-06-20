// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/inelastic/Glauber.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/inelastic/NucleonNucleonXsc.h"

namespace XS4GCR {

void GlauberTotalInel::print() const {
  LOGI << "using Glauber (simplified Grichine) total inelastic model:";
  LOGI << "Grichine, 2009, Eur. Phys. J. C 62, 399; NN cross sections from Norbury, 2008, NASA/TP-2008-215116";
}

std::shared_ptr<TotalInelastic> GlauberTotalInel::clone() { return std::make_shared<GlauberTotalInel>(*this); }

namespace {

double wilsonRmsRadius(const int& A) {
  double radius = 0;
  if (A > 26) {
    radius = 0.84 * pow(double(A), 1. / 3.) + 0.55;
  } else {
    std::vector<double> r = {0.0,   0.85, 2.095, 1.976, 1.671, 1.986, 2.57,  2.41,  2.23,
                             2.519, 2.45, 2.42,  2.471, 2.440, 2.58,  2.611, 2.730, 2.662,
                             2.727, 2.9,  3.040, 2.867, 2.969, 2.94,  3.075, 3.11,  3.06};
    radius = r.at(A);
  }
  return radius * cgs::fm;
}

// Coulomb-barrier suppression (Coulomb-modified Glauber): the cross section is reduced by
// (1 - B_C / T_cm) and vanishes below the barrier, following the G4 GlauberGribov prescription.
double coulombFactor(int Zp, int Ap, int Zt, int At, double Rp, double Rt, double T_n) {
  const double bC = 1.44 * cgs::MeV * cgs::fm * Zp * Zt / (Rp + Rt) / 2.0;  // e^2 = 1.44 MeV fm
  const double mP = Ap * cgs::protonMassC2;
  const double mT = At * cgs::protonMassC2;
  const double Elab = mP + Ap * T_n;
  const double s = mP * mP + mT * mT + 2. * mT * Elab;
  const double Tcm = std::sqrt(s) - mP - mT;
  if (Tcm <= bC) return 0.;
  return 1. - bC / Tcm;
}

}  // namespace

double GlauberTotalInel::getTotal(const PID& projectile, const TARGET& target, const double& T_n) const {
  assert(projectile != PID(1, 1));
  const int Ap = projectile.getA();
  const int Zp = projectile.getZ();
  const int Np = Ap - Zp;
  const int At = (target == TARGET::H) ? 1 : 4;
  const int Zt = (target == TARGET::H) ? 1 : 2;
  const int Nt = At - Zt;

  const double sigma_pp = NN::ppTotalXsc(T_n);  // = nn
  const double sigma_pn = NN::pnTotalXsc(T_n);

  // Sum the nucleon-nucleon cross sections over all projectile-target nucleon pairs, split
  // into like (pp, nn) and unlike (pn) pairs by isospin. This replaces the previous A*sigma_pp,
  // which ignored neutrons and used the (larger) total pp cross section for every pair.
  const double sigmaNN = (Zp * Zt + Np * Nt) * sigma_pp + (Zp * Nt + Np * Zt) * sigma_pn;

  const double Rp = wilsonRmsRadius(Ap);
  const double piR2 = M_PI * pow2(Rp);
  double value = piR2 * std::log(1. + sigmaNN / piR2);
  value *= coulombFactor(Zp, Ap, Zt, At, Rp, wilsonRmsRadius(At), T_n);
  return std::max(0., value);
}

}  // namespace XS4GCR
