#ifndef INCLUDE_XS4GCR_TERTIARY_AAFRAG_H
#define INCLUDE_XS4GCR_TERTIARY_AAFRAG_H

#include "XS4GCR/models/secAAfrag101.h"
#include "XS4GCR/tertiary/TertiaryProtons.h"

namespace XS4GCR {

class AAfragTerProtons : public TertiaryProtons {
 public:
  AAfragTerProtons();

  void print() const override;

  std::shared_ptr<TertiaryProtons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;

 protected:
  std::shared_ptr<AAfrag101::LookupTables> m_tables;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_TERTIARY_AAFRAG_H