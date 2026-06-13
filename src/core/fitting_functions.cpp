// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/core/fitting_functions.h"

#include <plog/Log.h>

#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <utility>

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

  input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

  while (input >> projectileZ >> projectileA >> childZ >> childA >> energyThreshold >> sigma0 >> mass >> gamma >>
         sigma1 >> xi >> delta >> relativeUncertainty) {
    (void)relativeUncertainty;

    const PID projectile(projectileZ, projectileA);
    const PID child(childZ, childA);
    const FragmentationChannel ch = std::make_pair(projectile, child);
    const SigmaFunction function(energyThreshold * cgs::MeV, sigma0 * cgs::mbarn, gamma * cgs::MeV,
                                 mass * cgs::MeV, sigma1 * cgs::mbarn, xi, delta);
    functions.insert(std::make_pair(ch, function));
  }

  LOGI << "read Evoli2026 fit parameters for " << functions.size() << " fragmentation channels";
}

}  // namespace XS4GCR
