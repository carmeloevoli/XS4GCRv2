#ifndef INCLUDE_XS4GCR_MODELS_SECAAFRAG101_H
#define INCLUDE_XS4GCR_MODELS_SECAAFRAG101_H

#include <plog/Log.h>

#include <string>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/grid.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

#define NHEADERLINES 0

namespace XS4GCR {
namespace AAfrag101 {

class AAfragTable {
 public:
  AAfragTable() {}
  AAfragTable(const std::string &filename, const std::vector<double> &xAxis, const std::vector<double> &yAxis)
      : m_filename(filename), m_xAxis(xAxis), m_yAxis(yAxis) {
    m_sigma = Grid<double>(m_xAxis.size(), m_yAxis.size());
    m_xRange = std::make_pair(m_xAxis.front(), m_xAxis.back());
    m_yRange = std::make_pair(m_yAxis.front(), m_yAxis.back());
  }

  virtual ~AAfragTable() = default;

  void readTable(size_t idColumn) {
    const auto sigma = UTILS::loadColumn(m_filename, idColumn, NHEADERLINES);
    m_sigma.copy(sigma);
    m_sigma.for_each([](double &s) { s *= cgs::mbarn; });
  }

  void readNuTables() {
    const auto sigma_nue = UTILS::loadColumn(m_filename, 2, NHEADERLINES);
    const auto sigma_anue = UTILS::loadColumn(m_filename, 3, NHEADERLINES);
    const auto sigma_numu = UTILS::loadColumn(m_filename, 4, NHEADERLINES);
    const auto sigma_anumu = UTILS::loadColumn(m_filename, 5, NHEADERLINES);
    for (size_t i = 0; i < m_sigma.size(); ++i)
      m_sigma.get(i) = sigma_nue.at(i) + sigma_anue.at(i) + sigma_numu.at(i) + sigma_anumu.at(i);
    m_sigma.for_each([](double &s) { s *= cgs::mbarn; });
  }

  double get(double x, double y) const {
    double value = 0;
    if (UTILS::inRange(x, m_xRange) && UTILS::inRange(y, m_yRange)) {
      auto z = m_sigma.get();
      value = GSL::interpolate2d<double>(m_xAxis, m_yAxis, z, x, y);
    }
    auto T_sec = std::pow(10., y);
    return value / T_sec;  // dsigma/dE_s
  }

 private:
  std::string m_filename;
  std::vector<double> m_xAxis;
  std::vector<double> m_yAxis;
  std::pair<double, double> m_xRange;
  std::pair<double, double> m_yRange;
  Grid<double> m_sigma;
};

enum class ParticleTypes { ELECTRON, POSITRON, GAMMA, ALLNUS, PBAR };
enum class Channel { pp, pHe, Hep, HeHe };

class LookupTables {
 public:
  LookupTables(ParticleTypes particle);
  virtual ~LookupTables() = default;
  double get(Channel channel, double T_proj, double T_sec) const;

 private:
  void initAntiprotonTables();
  void initGammaTables();
  void initNeutrinoTables();
  void initPositronTables(size_t idColumn = 3);
  void init();

 private:
  ParticleTypes m_particle;
  AAfragTable m_sigma_le_pp;
  AAfragTable m_sigma_le_pHe;
  AAfragTable m_sigma_le_Hep;
  AAfragTable m_sigma_le_HeHe;
  AAfragTable m_sigma_he_pp;
  AAfragTable m_sigma_he_pHe;
  AAfragTable m_sigma_he_Hep;
  AAfragTable m_sigma_he_HeHe;
};

}  // namespace AAfrag101
}  // namespace XS4GCR

#endif