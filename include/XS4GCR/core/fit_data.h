// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_FIT_DATA_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_FIT_DATA_H_

#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class FitData {
 public:
  struct DataPoint {
    FragmentationChannel channel;
    double T;
    double sigma;
    double sigmaError;
  };

  FitData() = default;
  explicit FitData(const std::string& filename) : filename(filename) { readData(); }

  bool hasChannel(const FragmentationChannel& ch) const {
    for (const auto& data : dataPoints)
      if (data.channel == ch) return true;
    return false;
  }

  std::vector<DataPoint> getData(const FragmentationChannel& ch) const {
    std::vector<DataPoint> result;
    for (const auto& data : dataPoints)
      if (data.channel == ch) result.push_back(data);
    if (result.empty()) throw std::runtime_error("Evoli2026 fit data channel not found");
    return result;
  }

 private:
  void readData() {
    std::ifstream input(filename.c_str());
    if (!input) throw std::runtime_error("problem with opening Evoli2026 fit-data file: " + filename);

    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int projectileZ = 0;
    int projectileA = 0;
    int fragmentZ = 0;
    int fragmentA = 0;
    double T = 0.;
    double sigma = 0.;
    double sigmaError = 0.;
    std::string label;

    while (input >> projectileZ >> projectileA >> fragmentZ >> fragmentA >> T >> sigma >> sigmaError >> label) {
      const FragmentationChannel ch = std::make_pair(PID(projectileZ, projectileA), PID(fragmentZ, fragmentA));
      dataPoints.push_back(DataPoint{ch, T * cgs::GeV, sigma * cgs::mbarn, sigmaError * cgs::mbarn});
    }
  }

  std::string filename;
  std::vector<DataPoint> dataPoints;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_FIT_DATA_H_
