#ifndef INCLUDE_XS4GCR_ANTIPROTONS_AAFRAG_H
#define INCLUDE_XS4GCR_ANTIPROTONS_AAFRAG_H

#include <memory>
#include <vector>

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"
#include "XS4GCR/models/secAAfrag101.h"

namespace XS4GCR {

class AAfragSecAp : public SecondaryAntiprotons {
 public:
  AAfragSecAp();

  void print() const override;

  std::shared_ptr<SecondaryAntiprotons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_ap) const override;

 protected:
  std::shared_ptr<AAfrag101::LookupTables> m_tables;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_ANTIPROTONS_AAFRAG_H