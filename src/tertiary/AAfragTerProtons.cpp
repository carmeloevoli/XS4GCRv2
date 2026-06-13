#include "XS4GCR/tertiary/AAfragTerProtons.h"

namespace XS4GCR {

AAfragTerProtons::AAfragTerProtons() { m_tables = std::make_shared<AAfrag202::LookupTables>(AAfrag202::Species::PROTON); }

void AAfragTerProtons::print() const {
  LOGI << "using AAfrag(v2.02) tertiary proton model: ";
  LOGI << "Kachelrieß, Ostapchenko & Tjemsland, 2023, Computer Physics Communications, Volume 287, article id. 108698";
}

std::shared_ptr<TertiaryProtons> AAfragTerProtons::clone() { return std::make_shared<AAfragTerProtons>(*this); }

double AAfragTerProtons::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                         const double& x) const {
  if (x > 1.) return 0.;
  const double T_p = x * T_proj;
  AAfrag202::Channel ch;
  if (!AAfrag202::channelFor(projectile.getZ(), projectile.getA(), target == TARGET::He, ch)) return 0.;
  return T_proj * m_tables->get(ch, T_proj, T_p);
}

}  // namespace XS4GCR
