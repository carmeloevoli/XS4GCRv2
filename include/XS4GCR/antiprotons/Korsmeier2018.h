#ifndef INCLUDE_XS4GCR_ANTIPROTONS_KORSMEIER2018_H
#define INCLUDE_XS4GCR_ANTIPROTONS_KORSMEIER2018_H

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"
#include "XS4GCR/core/grid.h"

namespace XS4GCR {

class Korsmeier2018SecAp : public SecondaryAntiprotons {
 public:
  Korsmeier2018SecAp();

  void print() const override;

  std::shared_ptr<SecondaryAntiprotons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n, const double &T_ap) const override;

 private:
  void init();
  void checkDatafilesExist();
  void readDataFiles();

 private:
  const std::string datafile = "data/Korsmeier2018/supplementary__XS_table_Param_II_B.txt";

  std::vector<double> m_lgTprojAxis;
  std::vector<double> m_lgTapAxis;

  Grid<double> m_sigma_pp;
  Grid<double> m_sigma_Hep;
  Grid<double> m_sigma_pHe;
  Grid<double> m_sigma_HeHe;
};

}  // namespace XS4GCR

#endif