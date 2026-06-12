#include "XS4GCR/fragmentations/Fluka4Dragon.h"

#include <plog/Log.h>

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

Fluka4Dragon::Fluka4Dragon(const std::string& dataFilename) : dataFilename(dataFilename) {
  if (!UTILS::fileExists(dataFilename)) throw std::runtime_error("problem with reading Fluka4Dragon data file");
  readDataFile();
}

void Fluka4Dragon::print() { LOGI << "using Fluka4Dragon fragmentation model: FLUKA 2021 tables for DRAGON2"; }

std::shared_ptr<Fragmentation> Fluka4Dragon::clone() { return std::make_shared<Fluka4Dragon>(*this); }

bool Fluka4Dragon::hasChannel(const FragmentationChannel& ch) const { return tables.find(ch) != tables.end(); }

double Fluka4Dragon::get(const FragmentationChannel& ch, const TARGET& target, const double& T_n, bool do_ghosts) {
  if (!do_ghosts) {
    throw std::runtime_error("Fluka4Dragon tables contain only cumulative cross-sections");
  }
  if (T_n <= 0.) return 0.;

  const auto it = tables.find(ch);
  if (it == tables.end()) return 0.;

  const double lgTnMeV = std::log(T_n / cgs::MeV);
  const auto& table = it->second;
  const auto& sigma = selectTargetTable(table, target);
  if (lgTnMeV < table.lgTnMeV.front()) return 0.;
  if (lgTnMeV >= table.lgTnMeV.back()) return std::max(0., sigma.back());
  return std::max(0., GSL::linearInterpolate<double>(table.lgTnMeV, sigma, lgTnMeV));
}

void Fluka4Dragon::readDataFile() {
  std::ifstream input(dataFilename.c_str());
  if (!input) throw std::runtime_error("problem with opening Fluka4Dragon data file");

  std::string line;
  size_t lineNumber = 0;
  while (std::getline(input, line)) {
    ++lineNumber;
    if (line.empty() || line[0] == '#') continue;

    std::istringstream row(line);
    std::string parentCode;
    std::string daughterCode;
    double energyMeV = 0.;
    double sigmaH = 0.;
    double sigmaHe = 0.;

    if (!(row >> parentCode >> daughterCode >> energyMeV >> sigmaH >> sigmaHe)) {
      throw std::runtime_error("malformed row in Fluka4Dragon data file at line " + std::to_string(lineNumber));
    }
    if (energyMeV <= 0.) {
      throw std::runtime_error("non-positive energy in Fluka4Dragon data file at line " + std::to_string(lineNumber));
    }

    const FragmentationChannel ch = std::make_pair(parsePidCode(parentCode), parsePidCode(daughterCode));
    auto& table = tables[ch];
    table.lgTnMeV.push_back(std::log(energyMeV));
    table.sigmaH.push_back(sigmaH * cgs::mbarn);
    table.sigmaHe.push_back(sigmaHe * cgs::mbarn);
  }

  if (tables.empty()) throw std::runtime_error("no channels loaded from Fluka4Dragon data file");
}

PID Fluka4Dragon::parsePidCode(const std::string& code) {
  const auto dot = code.find('.');
  if (dot == std::string::npos) throw std::runtime_error("malformed Fluka4Dragon nucleus code: " + code);

  const int Z = std::stoi(code.substr(0, dot));
  const int A = std::stoi(code.substr(dot + 1));
  return PID(Z, A);
}

const std::vector<double>& Fluka4Dragon::selectTargetTable(const Table& table, const TARGET& target) const {
  if (target == TARGET::H) return table.sigmaH;
  if (target == TARGET::He) return table.sigmaHe;
  throw std::runtime_error("target not implemented in Fluka4Dragon");
}

}  // namespace XS4GCR
