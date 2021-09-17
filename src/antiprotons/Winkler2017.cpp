#include "XS4GCR/antiprotons/Winkler2017.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/cgs.h"
#include "XS4GCR/utilities.h"

namespace XS4GCR {

Winkler2017SecAp::Winkler2017SecAp(const std::string& dataFilename) : m_dataFilename(dataFilename) {
  if (!UTILS::fileExist(m_dataFilename)) LOGE << "problem with reading data file " << m_dataFilename;
  m_Tn_range = std::make_pair(5.49540874e0 * cgs::GeV, 9.54992586e5 * cgs::GeV);
  m_Tap_range = std::make_pair(1e-2 * cgs::GeV, 1e3 * cgs::GeV);
  m_sigma_pp = Grid<double>(132, 251);
  m_sigma_pHe = Grid<double>(132, 251);
  m_sigma_Hep = Grid<double>(132, 251);
  m_sigma_HeHe = Grid<double>(132, 251);
  init();
}

void Winkler2017SecAp::init() {
  if (UTILS::countFileLines(m_dataFilename) != m_sigma_pp.size() + 1)
    LOGE << "the Winkler2017 datafile seems incorrect";

  const double units = cgs::mbarn / cgs::GeV;
  {
    const auto sigma_pp = UTILS::loadColumn(m_dataFilename, 2, 1);
    m_sigma_pp.copy(sigma_pp);
    m_sigma_pp.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma_pHe = UTILS::loadColumn(m_dataFilename, 3, 1);
    m_sigma_pHe.copy(sigma_pHe);
    m_sigma_pHe.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma_Hep = UTILS::loadColumn(m_dataFilename, 4, 1);
    m_sigma_Hep.copy(sigma_Hep);
    m_sigma_Hep.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma_HeHe = UTILS::loadColumn(m_dataFilename, 5, 1);
    m_sigma_HeHe.copy(sigma_HeHe);
    m_sigma_HeHe.for_each([units](double& s) { s *= units; });
  }
}

void Winkler2017SecAp::print() const {
  LOGI << "using Winkler2017 antiproton model: ";
  LOGI << "Winkler, 2017, Journal of Cosmology and Astroparticle Physics, Issue 02, article id. 048";
}

std::shared_ptr<SecondaryAntiprotons> Winkler2017SecAp::clone() { return std::make_shared<Winkler2017SecAp>(*this); }

double Winkler2017SecAp::get(const PID& projectile, const TARGET& target, const double& T_n, const double& T_ap) const {
  using std::log;
  double value = 0;
  if (UTILS::inRange(T_n, m_Tn_range) && UTILS::inRange(T_ap, m_Tap_range)) {
    auto f_Tn = UTILS::computeFraction(log(T_n), UTILS::logRange(m_Tn_range));
    auto f_Tap = UTILS::computeFraction(log(T_ap), UTILS::logRange(m_Tap_range));
    if (projectile == H1 && target == TARGET::H)
      value = m_sigma_pp.interpolate(f_Tn, f_Tap);
    else if (projectile == H1 && target == TARGET::He)
      value = m_sigma_pHe.interpolate(f_Tn, f_Tap);
    else if (projectile == He4 && target == TARGET::H)
      value = m_sigma_Hep.interpolate(f_Tn, f_Tap);
    else if (projectile == He4 && target == TARGET::He)
      value = m_sigma_HeHe.interpolate(f_Tn, f_Tap);
    else {
      LOGE << "channel not implemented in Winkler2017";
    }
  }
  return value;
}

}  // namespace XS4GCR
