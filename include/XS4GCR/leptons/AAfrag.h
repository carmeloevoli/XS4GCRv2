#ifndef INCLUDE_XS4GCR_LEPTONS_AAFRAG_H
#define INCLUDE_XS4GCR_LEPTONS_AAFRAG_H

#include <memory>
#include <vector>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/grid.h"
#include "XS4GCR/leptons/SecondaryLeptons.h"

namespace XS4GCR {

class AAfragSecLep : public SecondaryLeptons {
 public:
  AAfragSecLep(const PID &lepton);

  void print() const override;

  std::shared_ptr<SecondaryLeptons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_lepton) const override;

 protected:
  using Range = std::pair<double, double>;
  Range m_lgTprojRange;
  Range m_lgTapRange;

  Grid<double> m_sigma_pp;
  Grid<double> m_sigma_pHe;
  Grid<double> m_sigma_Hep;
  Grid<double> m_sigma_HeHe;
  std::string m_dataFilename;

 protected:
  void init();
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LEPTONS_AAFRAG_H