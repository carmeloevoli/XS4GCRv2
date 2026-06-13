#include "XS4GCR/fragmentations/UsineFragmentationTables.h"

#include <plog/Log.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

namespace {

std::string trim(const std::string& s) {
  const auto first = s.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) return "";

  const auto last = s.find_last_not_of(" \t\r\n");
  return s.substr(first, last - first + 1);
}

bool readNextDataLine(std::ifstream& input, std::string& line, size_t& lineNumber) {
  while (std::getline(input, line)) {
    ++lineNumber;
    line = trim(line);
    if (line.empty() || line[0] == '#') continue;
    return true;
  }
  return false;
}

std::string lineError(const std::string& message, size_t lineNumber) {
  return message + " at line " + std::to_string(lineNumber);
}

}  // namespace

namespace XS4GCR {

UsineFragmentation::UsineFragmentation(const std::string& dataFilename) : dataFilename(dataFilename) {
  if (!UTILS::fileExists(dataFilename)) throw std::runtime_error("problem with reading USINE fragmentation data file");
  readDataFile();
}

void UsineFragmentation::print() const { LOGI << "using USINE fragmentation table: " << dataFilename; }

std::shared_ptr<Fragmentation> UsineFragmentation::clone() { return std::make_shared<UsineFragmentation>(*this); }

bool UsineFragmentation::hasChannel(const FragmentationChannel& ch) const { return tables.find(ch) != tables.end(); }

double UsineFragmentation::getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n,
                               bool do_ghosts) {
  // USINE ghost handling is encoded by the selected table file.
  (void)do_ghosts;
  if (T_n <= 0.) return 0.;

  const auto it = tables.find(ch);
  if (it == tables.end()) return 0.;

  const auto& table = it->second;
  const auto& sigma = selectTargetTable(table, target);
  const double lgT = std::log(T_n / cgs::GeV);

  if (lgT < lgTnGeV.front()) return 0.;
  if (lgT >= lgTnGeV.back()) return std::max(0., sigma.back());

  return std::max(0., GSL::linearInterpolate<double>(lgTnGeV, sigma, lgT));
}

void UsineFragmentation::readDataFile() {
  std::ifstream input(dataFilename.c_str());
  if (!input) throw std::runtime_error("problem with opening USINE fragmentation data file");

  std::string line;
  size_t lineNumber = 0;
  std::string pendingLine;
  size_t pendingLineNumber = 0;
  bool hasPendingLine = false;
  auto readNext = [&]() {
    if (hasPendingLine) {
      line = pendingLine;
      lineNumber = pendingLineNumber;
      hasPendingLine = false;
      return true;
    }
    return readNextDataLine(input, line, lineNumber);
  };

  if (!readNext()) {
    throw std::runtime_error("missing USINE sigma definition");
  }

  std::string type;
  std::string unit;
  std::string targets;
  {
    std::istringstream row(line);
    if (!(row >> type >> unit >> targets)) {
      throw std::runtime_error(lineError("malformed USINE sigma definition", lineNumber));
    }
  }
  if (type != "sigma") {
    throw std::runtime_error(lineError("USINE fragmentation table must contain sigma values", lineNumber));
  }
  if (targets != "H,He") {
    throw std::runtime_error(lineError("USINE fragmentation table target list must be H,He", lineNumber));
  }
  const double sigmaUnit = unitFactor(unit);

  if (!readNext()) {
    throw std::runtime_error("missing USINE energy grid definition");
  }
  {
    std::istringstream row(line);
    if (!(row >> energyMinGeV >> energyMaxGeV >> energyCount)) {
      throw std::runtime_error(lineError("malformed USINE energy grid definition", lineNumber));
    }
  }
  if (!(energyMinGeV > 0.) || !(energyMaxGeV > energyMinGeV) || energyCount < 2) {
    throw std::runtime_error(lineError("invalid USINE energy grid definition", lineNumber));
  }

  lgTnGeV = UTILS::LogAxis(energyMinGeV, energyMaxGeV, energyCount);
  std::transform(lgTnGeV.begin(), lgTnGeV.end(), lgTnGeV.begin(), [](double T) { return std::log(T); });

  while (readNext()) {
    const size_t channelLineNumber = lineNumber;
    const auto arrow = line.find("->");
    if (arrow == std::string::npos) {
      throw std::runtime_error(lineError("malformed USINE fragmentation channel", channelLineNumber));
    }

    const auto projectile = trim(line.substr(0, arrow));
    const auto fragment = trim(line.substr(arrow + 2));
    const FragmentationChannel ch = std::make_pair(parseIsotopeCode(projectile), parseIsotopeCode(fragment));
    if (ch.second.getA() > ch.first.getA()) {
      while (readNextDataLine(input, line, lineNumber)) {
        if (line.find("->") == std::string::npos) continue;
        pendingLine = line;
        pendingLineNumber = lineNumber;
        hasPendingLine = true;
        break;
      }
      continue;
    }

    Table table;
    table.sigmaH.reserve(energyCount);
    table.sigmaHe.reserve(energyCount);
    for (size_t i = 0; i < energyCount; ++i) {
      if (!readNextDataLine(input, line, lineNumber)) {
        throw std::runtime_error(lineError("incomplete USINE fragmentation table", channelLineNumber));
      }

      double sigmaH = 0.;
      double sigmaHe = 0.;
      std::istringstream row(line);
      if (!(row >> sigmaH >> sigmaHe)) {
        throw std::runtime_error(lineError("malformed USINE fragmentation cross-section row", lineNumber));
      }

      table.sigmaH.push_back(sigmaH * sigmaUnit);
      table.sigmaHe.push_back(sigmaHe * sigmaUnit);
    }

    const auto inserted = tables.insert(std::make_pair(ch, std::move(table)));
    if (!inserted.second) {
      throw std::runtime_error(lineError("duplicate USINE fragmentation channel", channelLineNumber));
    }
  }

  if (tables.empty()) throw std::runtime_error("no channels loaded from USINE fragmentation data file");
}

PID UsineFragmentation::parseIsotopeCode(const std::string& code) {
  size_t split = 0;
  while (split < code.size() && std::isdigit(static_cast<unsigned char>(code[split]))) ++split;
  if (split == 0 || split == code.size()) throw std::runtime_error("malformed USINE isotope code: " + code);

  const int A = std::stoi(code.substr(0, split));
  const std::string symbol = code.substr(split);
  return PID(elementZ(symbol), A);
}

int UsineFragmentation::elementZ(const std::string& symbol) {
  static const std::map<std::string, int> elements = {
      {"H", 1},   {"He", 2},  {"Li", 3},  {"Be", 4},  {"B", 5},   {"C", 6},   {"N", 7},  {"O", 8},
      {"F", 9},   {"Ne", 10}, {"Na", 11}, {"Mg", 12}, {"Al", 13}, {"Si", 14}, {"P", 15}, {"S", 16},
      {"Cl", 17}, {"Ar", 18}, {"K", 19},  {"Ca", 20}, {"Sc", 21}, {"Ti", 22}, {"V", 23}, {"Cr", 24},
      {"Mn", 25}, {"Fe", 26}, {"Co", 27}, {"Ni", 28}, {"Cu", 29}, {"Zn", 30},
  };

  const auto it = elements.find(symbol);
  if (it == elements.end()) throw std::runtime_error("unknown USINE isotope symbol: " + symbol);
  return it->second;
}

double UsineFragmentation::unitFactor(const std::string& unit) {
  if (unit == "mb") return cgs::mbarn;
  if (unit == "cm2") return cgs::cm2;
  throw std::runtime_error("unsupported USINE fragmentation cross-section unit: " + unit);
}

const std::vector<double>& UsineFragmentation::selectTargetTable(const Table& table, const TARGET& target) const {
  if (target == TARGET::H) return table.sigmaH;
  if (target == TARGET::He) return table.sigmaHe;
  throw std::runtime_error("target not implemented in UsineFragmentation");
}

}  // namespace XS4GCR
