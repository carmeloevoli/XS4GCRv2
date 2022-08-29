#include "XS4GCR/core/lis.h"

#include <cmath>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {
namespace Boschini2017 {

double LIS::get(const double& T_n) const {
  double value = 0;
  auto A = static_cast<double>(id.second);
  auto Z = static_cast<double>(id.first);
  auto p = A * std::sqrt(T_n * (T_n + 2. * cgs::protonMassC2));
  auto logR_GV = std::log(p / Z / cgs::GeV);
  if (logR_GV > logR_table.front() && logR_GV < logR_table.back()) {
    auto logdJdR_GV = GSL::linearInterpolate<double>(logR_table, logdJdR_table, logR_GV);
    auto dJdR_GV = std::exp(logdJdR_GV);
    auto dRdT = A * (T_n + cgs::protonMassC2);
    dRdT /= Z * std::sqrt(T_n * T_n + 2. * cgs::protonMassC2 * T_n);
    value = dRdT * dJdR_GV;
  }
  return value / (cgs::GeV * cgs::m2 * cgs::second);
}

void LIS::read_filename() {
  if (UTILS::fileExists(filename)) {
    std::fstream file_to_read(filename);
    const int max_num_of_char_in_a_line = 512;
    const int num_of_header_lines = 15;
    for (int i = 0; i < num_of_header_lines; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');
    double R_read, dJdR_read;
    while (!file_to_read.eof()) {
      file_to_read >> R_read >> dJdR_read;
      if (!file_to_read.eof()) {
        logR_table.push_back(std::log(R_read));
        logdJdR_table.push_back(std::log(dJdR_read));
      }
    }
  } else {
    throw std::runtime_error("file for reading LIS cannot be found.");
  }
}

}  // namespace Boschini2017
}  // namespace XS4GCR