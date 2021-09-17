#ifndef INCLUDE_XS4GCR_ANTIPROTONS_DIMAURO2015_H
#define INCLUDE_XS4GCR_ANTIPROTONS_DIMAURO2015_H

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"

namespace XS4GCR {

class DiMauro2014SecAp : public SecondaryAntiprotons {
 public:
  DiMauro2014SecAp() {}

  void print() const override;

  std::shared_ptr<SecondaryAntiprotons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_n, const double &T_ap) const override;
};

}  // namespace XS4GCR

#endif