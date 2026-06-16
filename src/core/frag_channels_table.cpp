// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/core/frag_channels_table.h"

#include <plog/Log.h>

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "XS4GCR/core/cgs.h"

namespace {

std::string stripComment(const std::string& line) {
  const std::string::size_type pos = line.find('#');
  return (pos == std::string::npos) ? line : line.substr(0, pos);
}

std::string lineError(const std::string& filename, size_t lineNumber, const std::string& message) {
  return "malformed fragmentation direct-channel table '" + filename + "' at line " + std::to_string(lineNumber) +
         ": " + message;
}

double parseNonNegativeFinite(const std::string& filename, size_t lineNumber, const std::string& token) {
  size_t parsed = 0;
  double value = 0.;
  try {
    value = std::stod(token, &parsed);
  } catch (const std::exception&) {
    throw std::runtime_error(lineError(filename, lineNumber, "expected numeric value, got '" + token + "'"));
  }
  if (parsed != token.size()) {
    throw std::runtime_error(lineError(filename, lineNumber, "unexpected token '" + token + "'"));
  }
  if (!std::isfinite(value)) {
    throw std::runtime_error(lineError(filename, lineNumber, "numeric values must be finite"));
  }
  if (value < 0.) {
    throw std::runtime_error(lineError(filename, lineNumber, "numeric values must be non-negative"));
  }
  return value;
}

std::vector<double> readValues(const std::string& filename, size_t lineNumber, std::istringstream& row) {
  std::vector<double> values;
  std::string token;
  while (row >> token) values.push_back(parseNonNegativeFinite(filename, lineNumber, token));
  return values;
}

}  // namespace

namespace XS4GCR {

FragmentationChannels::FragmentationChannels(const std::string& filename) : filename(filename) { readDataFile(); }

bool FragmentationChannels::hasChannel(const FragmentationChannel& ch) const {
  return channels.find(ch) != channels.end();
}

const CrossSectionVector& FragmentationChannels::getCrossSections(const FragmentationChannel& ch) const {
  const auto it = channels.find(ch);
  if (it == channels.end()) throw std::runtime_error("fragmentation direct channel not found");
  return it->second;
}

const std::vector<double>& FragmentationChannels::getEnergies() const { return energies; }

const std::vector<double>& FragmentationChannels::getLogEnergies() const { return logEnergies; }

size_t FragmentationChannels::size() const { return channels.size(); }

void FragmentationChannels::readDataFile() {
  std::ifstream input(filename.c_str());
  if (!input) throw std::runtime_error("problem with opening fragmentation direct-channel table: " + filename);

  energies.clear();
  logEnergies.clear();
  channels.clear();

  std::string line;
  size_t lineNumber = 0;
  bool foundEnergyGrid = false;
  while (std::getline(input, line)) {
    ++lineNumber;

    std::istringstream row(stripComment(line));
    row >> std::ws;
    if (row.eof()) continue;

    int projectileA = 0;
    int projectileZ = 0;
    int fragmentA = 0;
    int fragmentZ = 0;
    if (!(row >> projectileA >> projectileZ >> fragmentA >> fragmentZ)) {
      throw std::runtime_error(lineError(filename, lineNumber, "expected four isotope columns"));
    }

    const auto values = readValues(filename, lineNumber, row);
    if (values.empty()) {
      throw std::runtime_error(lineError(filename, lineNumber, "missing energy or cross-section values"));
    }

    if (!foundEnergyGrid) {
      if (projectileA != 0 || projectileZ != 0 || fragmentA != 0 || fragmentZ != 0) {
        throw std::runtime_error(lineError(filename, lineNumber, "first data row must be the energy grid"));
      }
      for (const auto& energy : values) {
        if (energy <= 0.) throw std::runtime_error(lineError(filename, lineNumber, "energies must be positive"));
        energies.push_back(energy * cgs::GeV);
        logEnergies.push_back(std::log(energy * cgs::GeV));
      }
      foundEnergyGrid = true;
      continue;
    }

    if (projectileA < 1 || projectileZ < 1 || fragmentA < 1 || fragmentZ < 1) {
      throw std::runtime_error(lineError(filename, lineNumber, "isotope A and Z values must be positive"));
    }
    if (values.size() != energies.size()) {
      throw std::runtime_error(lineError(filename, lineNumber, "cross-section count does not match energy grid"));
    }

    CrossSectionVector crossSections;
    crossSections.reserve(values.size());
    for (const auto& sigma : values) crossSections.push_back(sigma * cgs::mbarn);

    const FragmentationChannel ch = std::make_pair(PID(projectileZ, projectileA), PID(fragmentZ, fragmentA));
    const auto inserted = channels.insert(std::make_pair(ch, crossSections));
    if (!inserted.second) throw std::runtime_error(lineError(filename, lineNumber, "duplicate channel"));
  }

  if (!foundEnergyGrid) {
    throw std::runtime_error("fragmentation direct-channel table has no energy grid: " + filename);
  }

  LOGI << "read fragmentation direct-channel table for " << channels.size() << " channels on " << energies.size()
       << " energies";
}

}  // namespace XS4GCR
