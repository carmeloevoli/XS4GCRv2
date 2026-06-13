#include "XS4GCR/antiprotons/AAfragSecAp.h"

namespace XS4GCR {

AAfragSecAp::AAfragSecAp() { m_tables = std::make_shared<AAfrag202::LookupTables>(AAfrag202::Species::ANTIPROTON); }

void AAfragSecAp::print() const {
  LOGI << "using AAfrag(v2.02) antiproton model: ";
  LOGI << "Kachelrieß, Ostapchenko & Tjemsland, 2023, Computer Physics Communications, Volume 287, article id. 108698";
}

std::shared_ptr<SecondaryAntiprotons> AAfragSecAp::clone() { return std::make_shared<AAfragSecAp>(*this); }

double AAfragSecAp::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                    const double& x) const {
  if (x > 1.) return 0.;
  const double T_ap = x * T_proj;
  AAfrag202::Channel ch;
  if (!AAfrag202::channelFor(projectile.getZ(), projectile.getA(), target == TARGET::He, ch)) return 0.;
  return T_proj * m_tables->get(ch, T_proj, T_ap);
}

}  // namespace XS4GCR
