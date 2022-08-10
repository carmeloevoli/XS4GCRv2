#include "XS4GCR/antiprotons/AAfragSecAp.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

AAfragSecAp::AAfragSecAp() {
  auto particle = AAfrag101::ParticleTypes::PBAR;
  m_tables = std::make_shared<AAfrag101::LookupTables>(particle);
}

void AAfragSecAp::print() const {
  LOGI << "using AAfrag(v101) antiproton model: ";
  LOGI << "Kachelrieß et al., 2019, Computer Physics Communications, Volume 245, article id. 106846";
}

std::shared_ptr<SecondaryAntiprotons> AAfragSecAp::clone() { return std::make_shared<AAfragSecAp>(*this); }

double AAfragSecAp::get(const PID& projectile, const TARGET& target, const double& T_proj, const double& T_ap) const {
  if (projectile == H1 && target == TARGET::H) {
    return m_tables->get(AAfrag101::Channel::pp, T_proj, T_ap);
  }
  return 0.;
}

}  // namespace XS4GCR
