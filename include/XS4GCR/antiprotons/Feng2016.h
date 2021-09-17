#ifndef INCLUDE_XS4GCR_ANTIPROTONS_FENG2016_H
#define INCLUDE_XS4GCR_ANTIPROTONS_FENG2016_H

#include <memory>
#include <vector>

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"
#include "XS4GCR/grid.h"

namespace XS4GCR {

class Feng2016SecAp : public SecondaryAntiprotons {
 public:
  Feng2016SecAp(const std::string &dataFilename);

  void print() const override;

  std::shared_ptr<SecondaryAntiprotons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n, const double &T_ap) const override;

 protected:
  using Range = std::pair<double, double>;
  Range m_Tn_range;
  Range m_Tap_range;
  Grid<double> m_sigma_pp;
  Grid<double> m_sigma_pHe;
  Grid<double> m_sigma_Hep;
  Grid<double> m_sigma_HeHe;
  std::string m_dataFilename;

 protected:
  void init();
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_ANTIPROTONS_FENG2016_H