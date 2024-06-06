#ifndef INCLUDE_XS4GCR_LEPTONS_ORUSA2022_H
#define INCLUDE_XS4GCR_LEPTONS_ORUSA2022_H

#include "XS4GCR/core/grid.h"
#include "XS4GCR/leptons/SecondaryLeptons.h"

namespace XS4GCR {

class Orusa2022SecLep : public SecondaryLeptons {
 public:
  explicit Orusa2022SecLep(const PID &lepton);

  void print() const override;

  std::shared_ptr<SecondaryLeptons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 private:
  void init();
  void checkDatafilesExist();
  void readDataFiles();

 private:
  const std::string eplus_datafile = "data/Orusa2022/supplementary_table_positrons_best_fit.dat";
  const std::string eminus_datafile = "data/Orusa2022/supplementary_table_electrons_best_fit.dat";

  std::vector<double> m_lgTprojAxis;
  std::vector<double> m_lgTsecAxis;

  Grid<double> m_sigma_pp;
  Grid<double> m_sigma_Hep;
  Grid<double> m_sigma_pHe;
  Grid<double> m_sigma_HeHe;
};

}  // namespace XS4GCR

#endif
