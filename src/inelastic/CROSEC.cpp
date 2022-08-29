#include "XS4GCR/inelastic/CROSEC.h"

#include <memory>

#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

double sighad_cc(int IS, double PA, double PZ, double TA, double TZ, double T) {
  return (sighad_(&IS, &PA, &PZ, &TA, &TZ, &T));
}

void sigtap_cc(int ISS) { sigtap2_(&ISS); }

void CrosecTotalInel::print() const {
  LOGI << "using CROSEC total inelastic model:";
  LOGI << "";
}

std::shared_ptr<TotalInelastic> CrosecTotalInel::clone() {
  init();
  return std::make_shared<CrosecTotalInel>(*this);
}

void CrosecTotalInel::init() {
  if (!UTILS::fileExists(dataFilename)) throw std::invalid_argument("CROSEC data file not read properly.");
  int ISS = -1;
  sigtap_cc(ISS);
}

double CrosecTotalInel::set_energy_within_range(const double& T_n) const {
  if (T_n < T_n_min)
    return T_n_min;
  else if (T_n > T_n_max)
    return T_n_max;
  else
    return T_n;
}

double CrosecTotalInel::get(const PID& projectile, const TARGET& target, const double& T_n) const {
  assert(projectile != PID(1, 1));
  double TA = projectile.getA();
  double TZ = projectile.getZ();                       // target nucleus mass and charge numbers (4.0<=A2<=239.0)
  double T = set_energy_within_range(T_n) / cgs::MeV;  // projectile particle kinetic energy (MEV; 14(20)MEV<T<1TEV)
  double value = sighad_cc(IS, PA, PZ, TA, TZ, T) * cgs::mbarn;
  value *= (target == TARGET::He) ? 2.1 * pow(projectile.getA(), .055) : 1;
  return value;
}

}  // namespace XS4GCR
