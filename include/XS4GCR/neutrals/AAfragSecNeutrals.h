#ifndef INCLUDE_XS4GCR_NEUTRALS_AAFRAG_H
#define INCLUDE_XS4GCR_NEUTRALS_AAFRAG_H

#include <memory>
#include <vector>

#include "XS4GCR/neutrals/SecondaryNeutrals.h"
#include "XS4GCR/shared/secAAfrag202.h"

namespace XS4GCR {

class AAfragSecNeutrals : public SecondaryNeutrals {
 public:
  AAfragSecNeutrals(NeutralParticleType type);

  void print() const override;

  std::shared_ptr<SecondaryNeutrals> clone() override;

  double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  std::shared_ptr<AAfrag202::LookupTables> m_tables;
};

using AAfragSecGammas = AAfragSecNeutrals;

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_NEUTRALS_AAFRAG_H
