#include "XS4GCR/nuclei/AAfragSecNuclei.h"

#include <stdexcept>
#include <string>

namespace XS4GCR {

namespace {

AAfrag202::Species toAAfragSpecies(NucleusSpecies species) {
  switch (species) {
    case NucleusSpecies::PROTON:       return AAfrag202::Species::PROTON;
    case NucleusSpecies::NEUTRON:      return AAfrag202::Species::NEUTRON;
    case NucleusSpecies::ANTINEUTRON:  return AAfrag202::Species::ANTINEUTRON;
    case NucleusSpecies::ANTIDEUTERON: return AAfrag202::Species::ANTIDEUTERON;
    case NucleusSpecies::ANTIHELIUM:   return AAfrag202::Species::ANTIHELIUM;
  }
  throw std::invalid_argument("unknown NucleusSpecies");
}

std::string speciesName(NucleusSpecies species) {
  switch (species) {
    case NucleusSpecies::PROTON:       return "proton";
    case NucleusSpecies::NEUTRON:      return "neutron";
    case NucleusSpecies::ANTINEUTRON:  return "antineutron";
    case NucleusSpecies::ANTIDEUTERON: return "antideuteron";
    case NucleusSpecies::ANTIHELIUM:   return "antihelium-3";
  }
  throw std::invalid_argument("unknown NucleusSpecies");
}

}  // namespace

AAfragSecNuclei::AAfragSecNuclei(NucleusSpecies species) : m_species(species) {
  m_tables = std::make_shared<AAfrag202::LookupTables>(toAAfragSpecies(species));
}

void AAfragSecNuclei::print() const {
  LOGI << "using AAfrag(v2.02) " << speciesName(m_species) << " model: ";
  LOGI << "Kachelrieß, Ostapchenko & Tjemsland, 2023, Computer Physics Communications, Volume 287, article id. 108698";
}

std::shared_ptr<SecondaryNuclei> AAfragSecNuclei::clone() { return std::make_shared<AAfragSecNuclei>(*this); }

double AAfragSecNuclei::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                        const double& x) const {
  if (x > 1.) return 0.;
  const double T_sec = x * T_proj;
  AAfrag202::Channel ch;
  if (!AAfrag202::channelFor(projectile.getZ(), projectile.getA(), target == TARGET::He, ch)) return 0.;
  return T_proj * m_tables->get(ch, T_proj, T_sec);
}

}  // namespace XS4GCR
