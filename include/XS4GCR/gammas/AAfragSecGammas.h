#ifndef INCLUDE_XS4GCR_GAMMAS_AAFRAG_H
#define INCLUDE_XS4GCR_GAMMAS_AAFRAG_H

#include <memory>
#include <vector>

#include "XS4GCR/gammas/Pi0Gammas.h"
#include "XS4GCR/models/secAAfrag101.h"

namespace XS4GCR {

class AAfragSecGammas : public Pi0Gammas {
 public:
  AAfragSecGammas();

  void print() const override;

  std::shared_ptr<Pi0Gammas> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_g) const override;

 protected:
  std::shared_ptr<AAfrag101::LookupTables> m_tables;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_GAMMAS_AAFRAG_H