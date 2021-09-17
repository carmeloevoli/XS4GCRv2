#include "XS4GCR/antiprotons/Winkler2017.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/cgs.h"
#include "XS4GCR/gsl.h"
#include "XS4GCR/utilities.h"

#define NHEADERS 1

namespace XS4GCR {

Winkler2017SecAp::Winkler2017SecAp(const std::string& dataFilename) : m_dataFilename(dataFilename) {
  if (!UTILS::fileExist(m_dataFilename)) throw std::runtime_error("problem with reading data file");
  m_lgTproj = UTILS::LinAxis(std::log(5.49540874e0 * cgs::GeV), std::log(9.54992586e5 * cgs::GeV), 132);
  m_lgTap = UTILS::LinAxis(std::log(1e-2 * cgs::GeV), std::log(1e3 * cgs::GeV), 251);
  m_sigma_pp = Grid<double>(132, 251);
  m_sigma_pHe = Grid<double>(132, 251);
  m_sigma_Hep = Grid<double>(132, 251);
  m_sigma_HeHe = Grid<double>(132, 251);
  init(cgs::mbarn / cgs::GeV);
}

void Winkler2017SecAp::init(double units) {
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

void Winkler2017SecAp::print() const {
  LOGI << "using Winkler2017 antiproton model: ";
  LOGI << "Winkler, 2017, Journal of Cosmology and Astroparticle Physics, Issue 02, article id. 048";
}

std::shared_ptr<SecondaryAntiprotons> Winkler2017SecAp::clone() { return std::make_shared<Winkler2017SecAp>(*this); }

double Winkler2017SecAp::get(const PID& projectile, const TARGET& target, const double& T_proj,
                             const double& T_ap) const {
  using std::log;

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
      throw std::runtime_error("channel not implemented in Winkler2017");
    }
  }

  return value;
}

}  // namespace XS4GCR
