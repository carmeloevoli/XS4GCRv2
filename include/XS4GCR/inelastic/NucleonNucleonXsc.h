// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_INELASTIC_NUCLEONNUCLEONXSC_H_
#define INCLUDE_XS4GCR_INELASTIC_NUCLEONNUCLEONXSC_H_

namespace XS4GCR {
namespace NN {

// Total (elastic + inelastic) nucleon-nucleon cross sections, parametrised over the full
// cosmic-ray energy range by Norbury (2008), NASA/TP-2008-215116, "Nucleon-Nucleon Total
// Cross Section" (which combines the HZETRN low-energy fit, an intermediate-energy fit, and
// the high-energy PDG form). T is the projectile kinetic energy in cgs energy units; the
// return value is a cross section in cgs area units.
double ppTotalXsc(const double& T);  // proton-proton (= neutron-neutron, by isospin symmetry)
double pnTotalXsc(const double& T);  // proton-neutron (= np)

}  // namespace NN
}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_INELASTIC_NUCLEONNUCLEONXSC_H_
