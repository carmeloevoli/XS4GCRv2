#ifndef INCLUDE_XS4GCR_UTILITIES_H
#define INCLUDE_XS4GCR_UTILITIES_H

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace XS4GCR {
namespace UTILS {

std::vector<double> LinAxis(const double& min, const double& max, const size_t& size);

std::vector<double> LogAxis(const double& min, const double& max, const size_t& size);

size_t countFileLines(const std::string& filename);

bool fileExist(const std::string& filename);

std::vector<double> loadColumn(const std::string& filename, size_t useCol, size_t nHeaderLines);

std::vector<std::string> split(std::string s, std::string delimiter);

bool inRange(double x, std::pair<double, double> range);

}  // namespace UTILS
}  // namespace XS4GCR

#endif