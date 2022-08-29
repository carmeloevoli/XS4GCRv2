#include "XS4GCR/core/utilities.h"

#include <cmath>
#include <fstream>
#include <sstream>

namespace XS4GCR {
namespace UTILS {

std::vector<double> LinAxis(const double& min, const double& max, const size_t& size) {
  if (!(min < max)) throw std::invalid_argument("min must be smaller than max");
  if (!(size > 1)) throw std::invalid_argument("size must be larger than 1");

  const double dx = (max - min) / (double)(size - 1);
  std::vector<double> v(size);
  for (size_t i = 0; i < size; ++i) {
    const auto value = min + dx * i;
    v[i] = value;
  }
  return v;
}

std::vector<double> LogAxis(const double& min, const double& max, const size_t& size) {
  if (!(min < max)) throw std::invalid_argument("min must be smaller than max");
  if (!(size > 1)) throw std::invalid_argument("size must be larger than 1");

  const double delta_log = std::exp(std::log(max / min) / (size - 1));
  std::vector<double> v(size);
  for (size_t i = 0; i < size; ++i) {
    const auto value = std::exp(std::log(min) + (double)i * std::log(delta_log));
    v[i] = value;
  }
  return v;
}

size_t countFileLines(const std::string& filename) {
  size_t count = 0;
  std::string line;
  std::ifstream file(filename.c_str());
  while (getline(file, line)) count++;
  return count;
}

bool fileExists(const std::string& filename) {
  std::ifstream f(filename.c_str());
  return f.good();
}

// std::vector<std::string> split(std::string s, std::string delimiter = " ") {
//   size_t pos_start = 0, pos_end, delim_len = delimiter.length();
//   std::string token;
//   std::vector<std::string> res;

//   while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
//     token = s.substr(pos_start, pos_end - pos_start);
//     pos_start = pos_end + delim_len;
//     res.push_back(token);
//   }

//   res.push_back(s.substr(pos_start));
//   return res;
// }

std::vector<std::string> split(std::string s, std::string delimiter = " ") {
  std::vector<std::string> result;
  std::istringstream iss(s);
  for (std::string s; iss >> s;) result.push_back(s);
  return result;
}

std::vector<double> loadColumn(const std::string& filename, size_t useCol, size_t nHeaderLines) {
  std::vector<double> v;
  std::string line;
  std::ifstream file(filename.c_str());
  size_t count = 0;
  while (getline(file, line)) {
    if (count >= nHeaderLines) {
      auto items = split(line, " ");
      if (items.size() > 0) {
        auto s = items.at(useCol);
        v.push_back(atof(s.c_str()));
      }
    }
    count++;
  }
  file.close();
  return v;
}

bool inRange(double x, std::pair<double, double> range) { return (x >= range.first && x <= range.second); }

}  // namespace UTILS
}  // namespace XS4GCR