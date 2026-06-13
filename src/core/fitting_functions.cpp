// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/core/fitting_functions.h"

#include <plog/Log.h>

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::string stripComment(const std::string& line) {
  const std::string::size_type pos = line.find('#');
  return (pos == std::string::npos) ? line : line.substr(0, pos);
}

std::string lineError(const std::string& filename, size_t lineNumber, const std::string& message) {
  return "malformed Evoli2026 fit-parameter file '" + filename + "' at line " + std::to_string(lineNumber) + ": " +
         message;
}

void requireFinite(const std::string& filename, size_t lineNumber, const std::string& name, double value) {
  if (!std::isfinite(value)) throw std::runtime_error(lineError(filename, lineNumber, name + " must be finite"));
}

void requireNonNegative(const std::string& filename, size_t lineNumber, const std::string& name, double value) {
  requireFinite(filename, lineNumber, name, value);
  if (value < 0.) throw std::runtime_error(lineError(filename, lineNumber, name + " must be non-negative"));
}

}  // namespace

namespace XS4GCR {

SigmaFunction::SigmaFunction(double energyThreshold, double sigma0, double gamma, double mass, double sigma1,
                             double xi, double delta)
    : energyThreshold(energyThreshold),
      sigma0(sigma0),
      gamma(gamma),
      mass(mass),
      sigma1(sigma1),
      xi(xi),
      delta(delta) {}

double SigmaFunction::sigmaLowEnergy(double T) const {
  double value = sigma0 * pow2(gamma) * pow2(T - energyThreshold);
  value /= pow2(pow2(T) - pow2(mass)) + pow2(gamma * mass);
  return value;
}

double SigmaFunction::sigmaHighEnergy(double T) const {
  const double transitionEnergy = 2. * cgs::GeV;
  double value = sigma1 * std::pow(1. - energyThreshold / T, xi);
  value *= 1. + delta / (1. + pow2(transitionEnergy / T));
  return value;
}

double SigmaFunction::get(double T) const {
  double value = 0.;
  if (T > energyThreshold) {
    value = sigmaHighEnergy(T);
    if (sigma0 > 0.) value += sigmaLowEnergy(T);
  }
  return value;
}

FittingFunctions::FittingFunctions(const std::string& filename) : dataFilename(filename) { readDataFile(); }

bool FittingFunctions::hasChannel(const FragmentationChannel& ch) const { return functions.find(ch) != functions.end(); }

double FittingFunctions::get(const FragmentationChannel& ch, double T_n) const {
  const auto it = functions.find(ch);
  if (it == functions.end()) return -1.;
  return it->second.get(T_n);
}

void FittingFunctions::readDataFile() {
  std::ifstream input(dataFilename.c_str());
  if (!input) throw std::runtime_error("problem with opening Evoli2026 fit-parameter file: " + dataFilename);

  functions.clear();

  std::string line;
  size_t lineNumber = 0;
  while (std::getline(input, line)) {
    ++lineNumber;

    std::istringstream row(stripComment(line));
    row >> std::ws;
    if (row.eof()) continue;

    int projectileZ = 0;
    int projectileA = 0;
    int childZ = 0;
    int childA = 0;
    double energyThreshold = 0.;
    double sigma0 = 0.;
    double mass = 0.;
    double gamma = 0.;
    double sigma1 = 0.;
    double xi = 0.;
    double delta = 0.;
    double relativeUncertainty = 0.;

    if (!(row >> projectileZ >> projectileA >> childZ >> childA >> energyThreshold >> sigma0 >> mass >> gamma >>
          sigma1 >> xi >> delta >> relativeUncertainty)) {
      throw std::runtime_error(lineError(dataFilename, lineNumber, "expected 12 columns"));
    }

    row >> std::ws;
    if (!row.eof()) {
      std::string token;
      row >> token;
      throw std::runtime_error(lineError(dataFilename, lineNumber, "unexpected token '" + token + "'"));
    }

    if (projectileZ < 1 || projectileA < 1) {
      throw std::runtime_error(lineError(dataFilename, lineNumber, "projectile Z and A must be positive"));
    }
    if (childZ < 1 || childA < 1) {
      throw std::runtime_error(lineError(dataFilename, lineNumber, "child Z and A must be positive"));
    }
    if (childA >= projectileA) {
      throw std::runtime_error(lineError(dataFilename, lineNumber, "child A must be smaller than projectile A"));
    }
    if (childZ > projectileZ) {
      throw std::runtime_error(lineError(dataFilename, lineNumber, "child Z cannot exceed projectile Z"));
    }

    requireNonNegative(dataFilename, lineNumber, "energy threshold", energyThreshold);
    requireNonNegative(dataFilename, lineNumber, "sigma0", sigma0);
    requireNonNegative(dataFilename, lineNumber, "mass", mass);
    requireNonNegative(dataFilename, lineNumber, "gamma", gamma);
    requireNonNegative(dataFilename, lineNumber, "sigma1", sigma1);
    requireFinite(dataFilename, lineNumber, "xi", xi);
    requireFinite(dataFilename, lineNumber, "delta", delta);
    requireNonNegative(dataFilename, lineNumber, "relative uncertainty", relativeUncertainty);
    if (sigma0 > 0. && gamma <= 0.) {
      throw std::runtime_error(lineError(dataFilename, lineNumber, "gamma must be positive when sigma0 is positive"));
    }

    const PID projectile(projectileZ, projectileA);
    const PID child(childZ, childA);
    const FragmentationChannel ch = std::make_pair(projectile, child);
    const SigmaFunction function(energyThreshold * cgs::MeV, sigma0 * cgs::mbarn, gamma * cgs::MeV,
                                 mass * cgs::MeV, sigma1 * cgs::mbarn, xi, delta);
    const auto inserted = functions.insert(std::make_pair(ch, function));
    if (!inserted.second) throw std::runtime_error(lineError(dataFilename, lineNumber, "duplicate channel"));
  }

  LOGI << "read Evoli2026 fit parameters for " << functions.size() << " fragmentation channels";
}

}  // namespace XS4GCR
