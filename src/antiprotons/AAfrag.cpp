#include "XS4GCR/antiprotons/AAfrag.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/cgs.h"
#include "XS4GCR/utilities.h"

namespace XS4GCR {

AAfragSecAp::AAfragSecAp(const std::string& dataFilename) : m_dataFilename(dataFilename) {
  // if (!UTILS::fileExist(m_dataFilename)) throw std::runtime_error("problem with reading data file");
  m_Tn_range = std::make_pair(5 * cgs::GeV, 993650.6125 * cgs::GeV);
  m_Tap_range = std::make_pair(0.01 * cgs::GeV, 974514.3431 * cgs::GeV);
  m_sigma_pp = Grid<double>(129, 25026);
  m_sigma_pHe = Grid<double>(132, 251);
  m_sigma_Hep = Grid<double>(132, 251);
  m_sigma_HeHe = Grid<double>(132, 251);
  init();
}

void AAfragSecAp::init() {
  //   if (UTILS::countFileLines(m_dataFilename) != m_sigma_pp.size() + 1)
  //     LOGE << "the Winkler2017 datafile seems incorrect";

  const double units = cgs::mbarn * cgs::eV;
  {
    const auto sigma_pp = UTILS::loadColumn(m_dataFilename, 1, 0);
    m_sigma_pp.copy(sigma_pp);
    m_sigma_pp.for_each([units](double& s) { s *= units; });
  }
  //{
  //     const auto sigma_pHe = UTILS::loadColumn(m_dataFilename, 3, 1);
  //     m_sigma_pHe.copy(sigma_pHe);
  //     m_sigma_pHe.for_each([units](double& s) { s *= units; });
  //   }
  //   {
  //     const auto sigma_Hep = UTILS::loadColumn(m_dataFilename, 4, 1);
  //     m_sigma_Hep.copy(sigma_Hep);
  //     m_sigma_Hep.for_each([units](double& s) { s *= units; });
  //   }
  //   {
  //     const auto sigma_HeHe = UTILS::loadColumn(m_dataFilename, 5, 1);
  //     m_sigma_HeHe.copy(sigma_HeHe);
  //     m_sigma_HeHe.for_each([units](double& s) { s *= units; });
  //   }
}

void AAfragSecAp::print() const {
  LOGI << "using AAfrag(v101) antiproton model: ";
  LOGI << "Kachelrieß et al., 2019, Computer Physics Communications, Volume 245, article id. 106846";
}

std::shared_ptr<SecondaryAntiprotons> AAfragSecAp::clone() { return std::make_shared<AAfragSecAp>(*this); }

double AAfragSecAp::get(const PID& projectile, const TARGET& target, const double& T_n, const double& T_ap) const {
  using std::log;
  double value = 0;
  if (UTILS::inRange(T_n, m_Tn_range) && UTILS::inRange(T_ap, m_Tap_range)) {
    auto f_Tn = UTILS::computeFraction(log(T_n), UTILS::logRange(m_Tn_range));
    auto f_Tap = UTILS::computeFraction(log(T_ap), UTILS::logRange(m_Tap_range));
    if (projectile == H1 && target == TARGET::H)
      value = m_sigma_pp.interpolate(f_Tn, f_Tap) / pow2(T_ap);
    else if (projectile == H1 && target == TARGET::He)
      value = m_sigma_pHe.interpolate(f_Tn, f_Tap);
    else if (projectile == He4 && target == TARGET::H)
      value = m_sigma_Hep.interpolate(f_Tn, f_Tap);
    else if (projectile == He4 && target == TARGET::He)
      value = m_sigma_HeHe.interpolate(f_Tn, f_Tap);
    else {
      LOGE << "channel not implemented in AAfragSecAp";
    }
  }
  return value;
}

}  // namespace XS4GCR
