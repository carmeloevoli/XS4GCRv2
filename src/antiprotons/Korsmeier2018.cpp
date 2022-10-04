#include "XS4GCR/antiprotons/Korsmeier2018.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/utilities.h"

#define NHEADERLINES 44

namespace XS4GCR {

Korsmeier2018SecAp::Korsmeier2018SecAp() {
  if (!UTILS::fileExists(datafile)) throw std::runtime_error("error in reading an input file for Korsmeier2018 model.");
  readDataFiles();
}

void Korsmeier2018SecAp::print() const {
  LOGI << "using KORSMEIER2018 antiproton model: ";
  LOGI << "Korsmeier et al., 2018, Physical Review D, Volume 97, Issue 10, id. 103019";
}

std::shared_ptr<SecondaryAntiprotons> Korsmeier2018SecAp::clone() {
  return std::make_shared<Korsmeier2018SecAp>(*this);
}

void Korsmeier2018SecAp::readDataFiles() {
  double TprojMin = 1.0 * cgs::GeV;
  double TprojMax = 1e7 * cgs::GeV;
  size_t TprojSize = 30 * 7 + 1;
  m_lgTprojAxis = UTILS::LinAxis(std::log(TprojMin), std::log(TprojMax), TprojSize);

  double TsecMin = 0.1 * cgs::GeV;
  double TsecMax = 1e4 * cgs::GeV;
  size_t TsecSize = 30 * 5 + 1;
  m_lgTapAxis = UTILS::LinAxis(std::log(TsecMin), std::log(TsecMax), TsecSize);

  m_sigma_pp = Grid<double>(TprojSize, TsecSize);
  m_sigma_Hep = Grid<double>(TprojSize, TsecSize);
  m_sigma_pHe = Grid<double>(TprojSize, TsecSize);
  m_sigma_HeHe = Grid<double>(TprojSize, TsecSize);

  double units = cgs::mbarn / cgs::GeV;
  {
    const auto sigma = UTILS::loadColumn(datafile, 2, NHEADERLINES);
    m_sigma_pp.copy(sigma);
    m_sigma_pp.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma = UTILS::loadColumn(datafile, 3, NHEADERLINES);
    m_sigma_pHe.copy(sigma);
    m_sigma_pHe.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma = UTILS::loadColumn(datafile, 4, NHEADERLINES);
    m_sigma_Hep.copy(sigma);
    m_sigma_Hep.for_each([units](double& s) { s *= units; });
  }
  {
    const auto sigma = UTILS::loadColumn(datafile, 5, NHEADERLINES);
    m_sigma_HeHe.copy(sigma);
    m_sigma_HeHe.for_each([units](double& s) { s *= units; });
  }
}

double Korsmeier2018SecAp::get(const PID& particle, const TARGET& target, const double& T_n, const double& T_ap) const {
  if (particle == H1 && target == TARGET::H) return 0;
}

}  // namespace XS4GCR