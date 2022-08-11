#include "XS4GCR/gammas/AAfragSecGammas.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

AAfragSecGammas::AAfragSecGammas() {
  auto particle = AAfrag101::ParticleTypes::GAMMA;
  m_tables = std::make_shared<AAfrag101::LookupTables>(particle);
}

void AAfragSecGammas::print() const {
  LOGI << "using AAfrag(v101) gamma model: ";
  LOGI << "Kachelrieß et al., 2019, Computer Physics Communications, Volume 245, article id. 106846";
}

std::shared_ptr<Pi0Gammas> AAfragSecGammas::clone() { return std::make_shared<AAfragSecGammas>(*this); }

double AAfragSecGammas::get(const PID& projectile, const TARGET& target, const double& T_proj,
                            const double& T_g) const {
  if (projectile == H1 && target == TARGET::H) {
    return m_tables->get(AAfrag101::Channel::pp, T_proj, T_g);
  } else if (projectile == H1 && target == TARGET::He) {
    return m_tables->get(AAfrag101::Channel::pHe, T_proj, T_g);
  } else if (projectile == He4 && target == TARGET::H) {
    return m_tables->get(AAfrag101::Channel::Hep, T_proj, T_g);
  } else if (projectile == He4 && target == TARGET::He) {
    return m_tables->get(AAfrag101::Channel::HeHe, T_proj, T_g);
  } else {
    throw std::runtime_error("channel not implemented in AAFRAG model");
  }
  return 0.;
}

}  // namespace XS4GCR
