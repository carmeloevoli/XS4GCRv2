#include "XS4GCR/leptons/Orusa2022.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

#define NHEADERLINES 42

Orusa2022SecLep::Orusa2022SecLep(const PID& lepton) {
  m_lepton = lepton;
  init();
}

void Orusa2022SecLep::print() const {
  LOGI << "using ORUSA2022 lepton production model:";
  LOGI << "Orusa et al., 2022, Phys. Rev. D 105, 123021";
}

std::shared_ptr<SecondaryLeptons> Orusa2022SecLep::clone() { return std::make_shared<Orusa2022SecLep>(*this); }

void Orusa2022SecLep::init() {
  checkDatafilesExist();
  readDataFiles();
}

void Orusa2022SecLep::checkDatafilesExist() {
  std::vector<std::string> inputfiles{eminus_datafile, eplus_datafile};
  std::for_each(inputfiles.begin(), inputfiles.end(), [](std::string s) {
    if (!UTILS::fileExists(s)) throw std::runtime_error("error in reading an input file for Orusa2022 model.");
  });
}

void Orusa2022SecLep::readDataFiles() {
  double TprojMin = 0.1 * cgs::GeV;
  double TprojMax = 1e6 * cgs::GeV;
  size_t TprojSize = 140;
  m_lgTprojAxis = UTILS::LinAxis(std::log(TprojMin), std::log(TprojMax), TprojSize);

  double TsecMin = 0.01 * cgs::GeV;
  double TsecMax = 1e4 * cgs::GeV;
  size_t TsecSize = 90;
  m_lgTsecAxis = UTILS::LinAxis(std::log(TsecMin), std::log(TsecMax), TsecSize);

  m_sigma_pp = Grid<double>(TprojSize, TsecSize);
  m_sigma_Hep = Grid<double>(TprojSize, TsecSize);
  m_sigma_pHe = Grid<double>(TprojSize, TsecSize);
  m_sigma_HeHe = Grid<double>(TprojSize, TsecSize);

  double units = cgs::mbarn / cgs::GeV;
  auto filename = (m_lepton == electron) ? eminus_datafile : eplus_datafile;

  {
    const auto sigma = UTILS::loadColumn(filename, 2, NHEADERLINES);
    m_sigma_pp.copy(sigma);
    m_sigma_pp.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma = UTILS::loadColumn(filename, 3, NHEADERLINES);
    m_sigma_pHe.copy(sigma);
    m_sigma_pHe.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma = UTILS::loadColumn(filename, 4, NHEADERLINES);
    m_sigma_Hep.copy(sigma);
    m_sigma_Hep.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma = UTILS::loadColumn(filename, 5, NHEADERLINES);
    m_sigma_HeHe.copy(sigma);
    m_sigma_HeHe.for_each([units](double& s) { s *= units; });
  }
}

double Orusa2022SecLep::get(const PID& projectile, const TARGET& target, const double& T_proj, const double& x) const {
  using std::log;
  double value = 0;

  const auto T_lepton = x * T_proj;
  const auto lgTproj = log(T_proj);
  const auto lgTprojRange = std::make_pair(m_lgTprojAxis.front(), m_lgTprojAxis.back());

  const auto lgTsec = log(T_lepton);
  const auto lgTsecRange = std::make_pair(m_lgTsecAxis.front(), m_lgTsecAxis.back());

  if (UTILS::inRange(lgTproj, lgTprojRange) && UTILS::inRange(lgTsec, lgTsecRange)) {
    if (projectile == H1 && target == TARGET::H) {
      auto z = m_sigma_pp.get();
      value = GSL::interpolate2d<double>(m_lgTprojAxis, m_lgTsecAxis, z, lgTproj, lgTsec);
    } else if (projectile == H1 && target == TARGET::He) {
      auto z = m_sigma_pHe.get();
      value = GSL::interpolate2d<double>(m_lgTprojAxis, m_lgTsecAxis, z, lgTproj, lgTsec);
    } else if (projectile == He4 && target == TARGET::H) {
      auto z = m_sigma_Hep.get();
      value = GSL::interpolate2d<double>(m_lgTprojAxis, m_lgTsecAxis, z, lgTproj, lgTsec);
    } else if (projectile == He4 && target == TARGET::He) {
      auto z = m_sigma_HeHe.get();
      value = GSL::interpolate2d<double>(m_lgTprojAxis, m_lgTsecAxis, z, lgTproj, lgTsec);
    } else {
      throw std::runtime_error("channel not implemented in Orusa2022 model");
    }
  }
  return T_proj * value;
}

}  // namespace XS4GCR