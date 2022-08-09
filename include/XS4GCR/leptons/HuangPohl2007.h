#ifndef INCLUDE_XS4GCR_LEPTONS_HUANGPOHL2007_H
#define INCLUDE_XS4GCR_LEPTONS_HUANGPOHL2007_H

#include "XS4GCR/core/grid.h"
#include "XS4GCR/leptons/SecondaryLeptons.h"

namespace XS4GCR {

class HuangPohl2007SecLep : public SecondaryLeptons {
 public:
  explicit HuangPohl2007SecLep(const PID &lepton);

  void print() const override;

  std::shared_ptr<SecondaryLeptons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_lepton) const override;

 private:
  void init();
  void checkDatafilesExist();
  void readDataFiles();

 private:
  const std::string eminus_p_datafile = "data/espectra_eminus.decay.p.matrix.final.data";
  const std::string eminus_he_datafile = "data/espectra_eminus.decay.he.matrix.final.data";
  const std::string eplus_p_datafile = "data/espectra_eplus.decay.p.matrix.final.data";
  const std::string eplus_he_datafile = "data/espectra_eplus.decay.he.matrix.final.data";
  const std::string prodxsec_p_datafile = "data/prodxsection.p.matrix.final.data";
  const std::string prodxsec_he_datafile = "data/prodxsection.he.matrix.final.data";

  std::vector<double> m_lgEproj;
  std::vector<double> m_lgTsec;

  Grid<double> m_sigma_pp;
  Grid<double> m_sigma_Hep;

  //     grid2d dsigma_dT_H;
  //     grid2d dsigma_dT_He;

  //     void read_data_file();
  //     void resize_sigma_grids();
};

}  // namespace XS4GCR

#endif
