#include "XS4GCR/leptons/AAfrag.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

AAfragSecLep::AAfragSecLep(const std::string& dataFilename) : m_dataFilename(dataFilename) {
  if (!UTILS::fileExist(m_dataFilename)) throw std::runtime_error("problem with reading data file");

  const double m_lgTprojMin = std::log(1e10 * cgs::eV);
  const double m_lgTprojMax = std::log(3.96868071e20 * cgs::eV);
  const double m_lgTapMin = std::log(81283.0547 * cgs::eV);
  const double m_lgTapMax = std::log(7.94328225e20 * cgs::eV);
  m_lgTprojRange = std::make_pair(m_lgTprojMin, m_lgTprojMax);
  m_lgTapRange = std::make_pair(m_lgTapMin, m_lgTapMax);

  m_sigma_pp = Grid<double>(54, 1600);
  m_sigma_pHe = Grid<double>(54, 1600);
  m_sigma_Hep = Grid<double>(54, 1600);
  m_sigma_HeHe = Grid<double>(54, 1600);
  init();
}

void AAfragSecLep::init() {
  LOGW << m_dataFilename << " " << UTILS::countFileLines(m_dataFilename) << " " << m_sigma_pp.size();
  if (UTILS::countFileLines(m_dataFilename) != m_sigma_pp.size() + 1) {
    LOGE << "the AAfrag datafile seems incorrect";
  }

  const double units = cgs::barn * cgs::GeV;
  {
    const auto sigma_pp = UTILS::loadColumn(m_dataFilename, 2, 1);
    m_sigma_pp.copy(sigma_pp);
    m_sigma_pp.for_each([units](double& s) { s *= units; });
  }
  // {
  //   const auto sigma_pHe = UTILS::loadColumn(m_dataFilename, 3, 1);
  //   m_sigma_pHe.copy(sigma_pHe);
  //   m_sigma_pHe.for_each([units](double& s) { s *= units; });
  // }
  // {
  //   const auto sigma_Hep = UTILS::loadColumn(m_dataFilename, 4, 1);
  //   m_sigma_Hep.copy(sigma_Hep);
  //   m_sigma_Hep.for_each([units](double& s) { s *= units; });
  // }
  // {
  //   const auto sigma_HeHe = UTILS::loadColumn(m_dataFilename, 5, 1);
  //   m_sigma_HeHe.copy(sigma_HeHe);
  //   m_sigma_HeHe.for_each([units](double& s) { s *= units; });
  // }
}

void AAfragSecAp::print() const {
  LOGI << "using AAfrag(v101) antiproton model: ";
  LOGI << "Kachelrieß et al., 2019, Computer Physics Communications, Volume 245, article id. 106846";
}

std::shared_ptr<SecondaryLeptons> AAfragSecLep::clone() { return std::make_shared<AAfragSecLep>(*this); }

double AAfragSecLep::get(const PID& projectile, const TARGET& target, const double& T_proj,
                         const double& T_lepton) const {
  double value = 0;

  auto lgTproj = std::log(T_proj);
  auto lgTap = std::log(T_ap);

  if (UTILS::inRange(lgTproj, m_lgTprojRange) && UTILS::inRange(lgTap, m_lgTapRange)) {
    auto x_Tp = (lgTproj - m_lgTprojRange.first) / (m_lgTprojRange.second - m_lgTprojRange.first);
    auto y_Tap = (lgTap - m_lgTapRange.first) / (m_lgTapRange.second - m_lgTapRange.first);
    if (projectile == H1 && target == TARGET::H) {
      value = m_sigma_pp.interpolate(x_Tp, y_Tap) / T_ap;
    }
    //   else if (projectile == H1 && target == TARGET::He)
    //     value = m_sigma_pHe.interpolate(f_Tp, f_Tap);
    //   else if (projectile == He4 && target == TARGET::H)
    //     value = m_sigma_Hep.interpolate(f_Tp, f_Tap);
    //   else if (projectile == He4 && target == TARGET::He)
    //     value = m_sigma_HeHe.interpolate(f_Tp, f_Tap);
    //   else {
    //     LOGE << "channel not implemented in AAfrag model";
    //   }
  }
  return value;
}

}  // namespace XS4GCR
