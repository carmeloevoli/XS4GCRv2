#ifndef INCLUDE_XS4GCR_NUCLEICHART_H
#define INCLUDE_XS4GCR_NUCLEICHART_H

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class NucleiChart {
 public:
  enum DecayMode { STABLE, BETA_MINUS, BETA_PLUS, BETA_MINUS_FED, BETA_PLUS_FED };
  using decayParams = std::pair<DecayMode, double>;

  NucleiChart();
  virtual ~NucleiChart() = default;

  void print();
  std::shared_ptr<NucleiChart> clone();

  double getHalftime(PID particle) const;
  std::string getMode(PID particle) const;
  //   std::vector<PID> get_particle_list() const;

 protected:
  void readTable();
  void addIsotope(const std::string& line);

 protected:
  const std::string m_chartFilename = "data/crchart_Z28_2020.txt";
  std::map<PID, decayParams> m_chart;
};

// class DefaultCosmicRayChart : public CosmicRayChart {
//  public:
//   DefaultCosmicRayChart();
//   void print() override;
//   void init() override;
//   std::shared_ptr<CosmicRayChart> clone() override;

//  protected:
// };

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_NUCLEICHART_H
