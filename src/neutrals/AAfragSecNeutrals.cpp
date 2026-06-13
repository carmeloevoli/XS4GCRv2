#include "XS4GCR/neutrals/AAfragSecNeutrals.h"

namespace XS4GCR {

AAfragSecNeutrals::AAfragSecNeutrals(NeutralParticleType type) {
  const auto species = (type == NeutralParticleType::GAMMA) ? AAfrag202::Species::GAMMA : AAfrag202::Species::ALLNUS;
  m_tables = std::make_shared<AAfrag202::LookupTables>(species);
}

void AAfragSecNeutrals::print() const {
  LOGI << "using AAfrag(v2.02) gamma/neutrino model: ";
  LOGI << "Kachelrieß, Ostapchenko & Tjemsland, 2023, Computer Physics Communications, Volume 287, article id. 108698";
}

std::shared_ptr<SecondaryNeutrals> AAfragSecNeutrals::clone() { return std::make_shared<AAfragSecNeutrals>(*this); }

double AAfragSecNeutrals::getDifferential(const PID& projectile, const TARGET& target, const double& T_proj,
                                          const double& x) const {
  if (x > 1.) return 0.;
  const double T_sec = x * T_proj;
  AAfrag202::Channel ch;
  if (!AAfrag202::channelFor(projectile.getZ(), projectile.getA(), target == TARGET::He, ch)) return 0.;
  return T_proj * m_tables->get(ch, T_proj, T_sec);
}

}  // namespace XS4GCR
