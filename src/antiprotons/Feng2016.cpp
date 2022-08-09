#include "XS4GCR/antiprotons/Feng2016.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

#define NHEADERS 1

namespace XS4GCR {

Feng2016SecAp::Feng2016SecAp(const std::string& dataFilename) : m_dataFilename(dataFilename) {
  if (!UTILS::fileExist(m_dataFilename)) throw std::runtime_error("problem with reading data file");
  m_lgTproj = UTILS::LinAxis(std::log(5 * cgs::GeV), std::log(9.93651e5 * cgs::GeV), 129);
  m_lgTap = UTILS::LinAxis(std::log(0.01 * cgs::GeV), std::log(1.98730e3 * cgs::GeV), 129);
  m_sigma_pp = Grid<double>(129, 129);
  m_sigma_pHe = Grid<double>(129, 129);
  m_sigma_Hep = Grid<double>(129, 129);
  m_sigma_HeHe = Grid<double>(129, 129);
  init(cgs::barn / cgs::GeV);
}

void Feng2016SecAp::init(double units) {
  if (UTILS::countFileLines(m_dataFilename) != m_sigma_pp.size() + NHEADERS)
    throw std::runtime_error("the datafile seems incompatible with the grid size");
  {
    const auto sigma_pp = UTILS::loadColumn(m_dataFilename, 2, NHEADERS);
    m_sigma_pp.copy(sigma_pp);
    m_sigma_pp.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma_pHe = UTILS::loadColumn(m_dataFilename, 3, NHEADERS);
    m_sigma_pHe.copy(sigma_pHe);
    m_sigma_pHe.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma_Hep = UTILS::loadColumn(m_dataFilename, 4, NHEADERS);
    m_sigma_Hep.copy(sigma_Hep);
    m_sigma_Hep.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma_HeHe = UTILS::loadColumn(m_dataFilename, 5, NHEADERS);
    m_sigma_HeHe.copy(sigma_HeHe);
    m_sigma_HeHe.for_each([units](double& s) { s *= units; });
  }
}

void Feng2016SecAp::print() const {
  LOGI << "using Feng2016 antiproton model: ";
  LOGI << "Feng et al., 2016, Physical Review D, Volume 94, Issue 12, id.123007";
}

std::shared_ptr<SecondaryAntiprotons> Feng2016SecAp::clone() { return std::make_shared<Feng2016SecAp>(*this); }

double Feng2016SecAp::get(const PID& projectile, const TARGET& target, const double& T_proj, const double& T_ap) const {
  using std::log;  // TODO check if sigma is ds/dT or ds/dp and correct

  const double lgT_proj = log(T_proj);
  const double lgT_ap = log(T_ap);
  const auto lgT_proj_range = std::make_pair(m_lgTproj.front(), m_lgTproj.back());
  const auto lgT_ap_range = std::make_pair(m_lgTap.front(), m_lgTap.back());

  double value = 0;

  if (UTILS::inRange(lgT_proj, lgT_proj_range) && UTILS::inRange(lgT_ap, lgT_ap_range)) {
    if (projectile == H1 && target == TARGET::H) {
      auto z = m_sigma_pp.get();
      value = GSL::interpolate2d<double>(m_lgTproj, m_lgTap, z, lgT_proj, lgT_ap);
    } else if (projectile == H1 && target == TARGET::He) {
      auto z = m_sigma_pHe.get();
      value = GSL::interpolate2d<double>(m_lgTproj, m_lgTap, z, lgT_proj, lgT_ap);
    } else if (projectile == He4 && target == TARGET::H) {
      auto z = m_sigma_Hep.get();
      value = GSL::interpolate2d<double>(m_lgTproj, m_lgTap, z, lgT_proj, lgT_ap);
    } else if (projectile == He4 && target == TARGET::He) {
      auto z = m_sigma_HeHe.get();
      value = GSL::interpolate2d<double>(m_lgTproj, m_lgTap, z, lgT_proj, lgT_ap);
    } else {
      throw std::runtime_error("channel not implemented in Feng2016");
    }
  }

  return value;
}

}  // namespace XS4GCR
