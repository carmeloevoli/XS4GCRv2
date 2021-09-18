#ifndef INCLUDE_XS4GCR_INELASTIC_CROSEC_H
#define INCLUDE_XS4GCR_INELASTIC_CROSEC_H

#include "XS4GCR/cgs.h"
#include "XS4GCR/inelastic/inelastic.h"
#include "XS4GCR/pid.h"

namespace XS4GCR {

extern "C" double sighad_(int *, double *, double *, double *, double *, double *);
double sighad_cc(int IS, double PA, double PZ, double TA, double TZ, double T);

extern "C" void sigtap2_(int *);
void sigtap_cc(int);

class CrosecTotalInel : public TotalInelastic {
 public:
  CrosecTotalInel() {}

  void print() const override;

  std::shared_ptr<TotalInelastic> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n) const override;

 protected:
  void init();

  double set_energy_within_range(const double &T_n) const;

  const std::string dataFilename = "data/barpol.txt";
  const int IS = 2;  // calculation of non-elastic cross-sections
  double PA = 1;
  double PZ = 1;  // projectile mass and charge numbers (for pions PA<0.2)
  const double T_n_min = 20. * cgs::MeV;
  const double T_n_max = cgs::TeV;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_CROSEC_H_
