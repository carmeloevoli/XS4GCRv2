#include "XS4GCR/leptons/AAfragSecLeptons.h"

namespace XS4GCR {

AAfragSecLeptons::AAfragSecLeptons(const PID& lepton) {
  const auto species = (lepton == positron) ? AAfrag202::Species::POSITRON : AAfrag202::Species::ELECTRON;
  m_tables = std::make_shared<AAfrag202::LookupTables>(species);
}

void AAfragSecLeptons::print() const {
  LOGI << "using AAfrag(v2.02) lepton model: ";
  LOGI << "Kachelrieß, Ostapchenko & Tjemsland, 2023, Computer Physics Communications, Volume 287, article id. 108698";
}

std::shared_ptr<SecondaryLeptons> AAfragSecLeptons::clone() { return std::make_shared<AAfragSecLeptons>(*this); }

double AAfragSecLeptons::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                         const double& x) const {
  if (x > 1.) return 0.;
  const double T_lepton = x * T_proj;
  AAfrag202::Channel ch;
  if (!AAfrag202::channelFor(projectile.getZ(), projectile.getA(), target == TARGET::He, ch)) return 0.;
  return T_proj * m_tables->get(ch, T_proj, T_lepton);
}

}  // namespace XS4GCR
