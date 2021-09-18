#include "XS4GCR/inelastic/Tripathi1999.h"

#include "XS4GCR/models/inTripathi99.h"

namespace XS4GCR {

void Tripathi1999TotalInel::print() const {
  LOGI << "using Tripathi+99 total inelastic model:";
  LOGI << " ";
}

std::shared_ptr<TotalInelastic> Tripathi1999TotalInel::clone() {
  return std::make_shared<Tripathi1999TotalInel>(*this);
}

double Tripathi1999TotalInel::get(const PID& projectile, const TARGET& target, const double& T_n) const {
  assert(projectile != PID(1, 1));
  int A_p = (target == TARGET::H) ? 1 : 4;
  int Z_p = (target == TARGET::H) ? 1 : 2;
  int A_t = projectile.getA();
  int Z_t = projectile.getZ();

  return Tripathi99::inelastic_sigma(A_p, Z_p, A_t, Z_t, T_n);
}

}  // namespace XS4GCR
