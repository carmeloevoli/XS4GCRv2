#include "XS4GCR/tertiary/AAfragTerProtons.h"

namespace XS4GCR {

AAfragTerProtons::AAfragTerProtons() {
  auto particle = AAfrag101::ParticleTypes::PROTON;
  m_tables = std::make_shared<AAfrag101::LookupTables>(particle);
}

void AAfragTerProtons::print() const {
  LOGI << "using AAfrag(v101) gamma model: ";
  LOGI << "Kachelrieß et al., 2019, Computer Physics Communications, Volume 245, article id. 106846";
}

std::shared_ptr<TertiaryProtons> AAfragTerProtons::clone() { return std::make_shared<AAfragTerProtons>(*this); }

double AAfragTerProtons::get(const PID& projectile, const TARGET& target, const double& T_proj,
                             const double& T_p) const {
  if (projectile == H1 && target == TARGET::H) {
    return m_tables->get(AAfrag101::Channel::pp, T_proj, T_p);
  } else if (projectile == H1 && target == TARGET::He) {
    return m_tables->get(AAfrag101::Channel::pHe, T_proj, T_p);
  } else if (projectile == He4 && target == TARGET::H) {
    return m_tables->get(AAfrag101::Channel::Hep, T_proj, T_p);
  } else if (projectile == He4 && target == TARGET::He) {
    return m_tables->get(AAfrag101::Channel::HeHe, T_proj, T_p);
  } else {
    throw std::runtime_error("channel not implemented in AAFRAG model");
  }
  return 0.;
}

}  // namespace XS4GCR
