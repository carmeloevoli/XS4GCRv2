#include "XS4GCR/leptons/AAfragSecLeptons.h"

namespace XS4GCR {

AAfragSecLeptons::AAfragSecLeptons(const PID& lepton) {
  auto particle = (lepton == positron) ? AAfrag101::ParticleTypes::POSITRON : AAfrag101::ParticleTypes::ELECTRON;
  m_tables = std::make_shared<AAfrag101::LookupTables>(particle);
}

void AAfragSecLeptons::print() const {
  LOGI << "using AAfrag(v101) gamma model: ";
  LOGI << "Kachelrieß et al., 2019, Computer Physics Communications, Volume 245, article id. 106846";
}

std::shared_ptr<SecondaryLeptons> AAfragSecLeptons::clone() { return std::make_shared<AAfragSecLeptons>(*this); }

double AAfragSecLeptons::get(const PID& projectile, const TARGET& target, const double& T_proj,
                             const double& T_g) const {
  if (projectile == H1 && target == TARGET::H) {
    return m_tables->get(AAfrag101::Channel::pp, T_proj, T_g);
  }
  return 0.;
}

}  // namespace XS4GCR
