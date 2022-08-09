#include "XS4GCR/core/nucleiChart.h"

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

void NucleiChart::print() { LOGI << "using DEFAULT nuclei chart"; }

NucleiChart::NucleiChart() {
  if (!UTILS::fileExist(m_chartFilename)) throw std::runtime_error("error in reading nuclei chart file.");
  readTable();
}

void NucleiChart::readTable() {
  std::string line;
  std::ifstream in(m_chartFilename);
  while (std::getline(in, line))
    if (line.length() > 0 && line[0] != '#') {
      addIsotope(line);
    }
  LOGI << "read CR nuclei chart with " << m_chart.size() << " isotopes";
}

void NucleiChart::addIsotope(const std::string& line) {
  int A, Z;
  std::string name, mode;
  double tau;
  std::istringstream ss(line);
  ss >> Z >> A >> name >> mode >> tau;
  tau *= cgs::kyr;
  DecayMode decayMode;
  if (mode == "STABLE") {
    decayMode = STABLE;
  } else if (mode == "BETA-") {
    decayMode = BETA_MINUS;
  } else if (mode == "BETA+") {
    decayMode = BETA_PLUS;
  } else if (mode == "BETA-FED") {
    decayMode = BETA_MINUS_FED;
  } else if (mode == "BETA+FED") {
    decayMode = BETA_PLUS_FED;
  } else {
    throw std::invalid_argument("decay mode not valid");
  }
  auto pid = PID(Z, A);
  decayParams params{decayMode, tau};
  auto ret = m_chart.insert(std::make_pair(pid, params));
  if (ret.second == false) {
    LOGW << "the element " << pid << " already exists in the chart";
  }
}

std::shared_ptr<NucleiChart> NucleiChart::clone() { return std::make_shared<NucleiChart>(*this); }

double NucleiChart::getHalftime(PID particle) const {
  auto it = m_chart.find(particle);
  if (it != m_chart.end()) {
    return it->second.second;
  } else {
    LOGE << "particle " << particle << " not found in chart";
    return -1;
  }
}

std::string NucleiChart::getMode(PID particle) const {
  auto it = m_chart.find(particle);
  if (it != m_chart.end()) {
    auto mode = it->second.first;
    if (mode == BETA_MINUS) {
      return "B-";
    } else if (mode == BETA_MINUS_FED) {
      return "B-FED";
    } else if (mode == BETA_PLUS) {
      return "B+";
    } else if (mode == BETA_PLUS_FED) {
      return "B+FED";
    } else {
      return "STABLE";
    }
  } else {
    LOGE << "particle " << particle << " not found in chart";
    return "none";
  }
}

// std::vector<PID> CosmicRayChart::get_particle_list() const {
//     std::vector<PID> list;
//     for (auto p : m_chart) list.push_back(p.first);
//     std::sort(list.begin(), list.end());
//     return list;
// }

}  // namespace XS4GCR
