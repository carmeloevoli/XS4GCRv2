#ifndef INCLUDE_XS4GCR_ANTIPROTONS_TANNG1983_H
#define INCLUDE_XS4GCR_ANTIPROTONS_TANNG1983_H

#include <memory>

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"

namespace XS4GCR {

class TanNg1983SecAp : public SecondaryAntiprotons {
 public:
  TanNg1983SecAp() {}

  void print() const override;

  std::shared_ptr<SecondaryAntiprotons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &x) const override;
};

}  // namespace XS4GCR

#endif