// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_FITTING_FUNCTIONS_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_FITTING_FUNCTIONS_H_

#include <map>
#include <string>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

class SigmaFunction {
 public:
  SigmaFunction(double energyThreshold, double sigma0, double gamma, double mass, double sigma1, double xi,
                double delta);

  double get(double T) const;

 private:
  double sigmaLowEnergy(double T) const;
  double sigmaHighEnergy(double T) const;

  double energyThreshold;
  double sigma0;
  double gamma;
  double mass;
  double sigma1;
  double xi;
  double delta;
};

class FittingFunctions {
 public:
  FittingFunctions() = default;
  explicit FittingFunctions(const std::string& filename);

  bool hasChannel(const FragmentationChannel& ch) const;
  double get(const FragmentationChannel& ch, double T_n) const;

 private:
  void readDataFile();

  std::string dataFilename;
  std::map<FragmentationChannel, SigmaFunction> functions;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_FITTING_FUNCTIONS_H_
