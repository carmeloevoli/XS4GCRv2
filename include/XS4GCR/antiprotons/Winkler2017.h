#ifndef INCLUDE_XS4GCR_ANTIPROTONS_WINKLER2017_H
#define INCLUDE_XS4GCR_ANTIPROTONS_WINKLER2017_H

#include <memory>
#include <vector>

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"
#include "XS4GCR/grid.h"

namespace XS4GCR {

class Winkler2017SecAp : public SecondaryAntiprotons {
 public:
  Winkler2017SecAp(const std::string &dataFilename);

  void print() const override;

  std::shared_ptr<SecondaryAntiprotons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_ap) const override;

 protected:
  std::vector<double> m_lgTproj;
  std::vector<double> m_lgTap;

  Grid<double> m_sigma_pp;
  Grid<double> m_sigma_pHe;
  Grid<double> m_sigma_Hep;
  Grid<double> m_sigma_HeHe;

  std::string m_dataFilename;

 protected:
  void init(double units);
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_ANTIPROTONS_WINKLER2017_H