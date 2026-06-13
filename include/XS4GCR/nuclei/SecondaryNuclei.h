#ifndef INCLUDE_XS4GCR_NUCLEI_SECONDARYNUCLEI_H
#define INCLUDE_XS4GCR_NUCLEI_SECONDARYNUCLEI_H

#include <memory>

#include "XS4GCR/core/XSecModel.h"
#include "XS4GCR/core/pid.h"

namespace XS4GCR {

// Light nuclear secondaries tabulated by AAfrag v2.02 but without a dedicated
// parametrization of their own: protons, neutrons/antineutrons, antideuterons,
// antihelium-3.
enum class NucleusSpecies { PROTON, NEUTRON, ANTINEUTRON, ANTIDEUTERON, ANTIHELIUM };

class SecondaryNuclei : public XSecModel<SecondaryNuclei> {
 public:
  // Differential cross-section for a secondary (anti)nucleus carrying fraction
  // x = T_sec/T_proj of a projectile of kinetic energy per nucleon T_proj on a target.
  virtual double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj,
                                 const double &x) const = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_NUCLEI_SECONDARYNUCLEI_H
