// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_WEBBER1993_H_
#define INCLUDE_XS4GCR_FRAGMENTATIONS_WEBBER1993_H_

#include <memory>

#include "XS4GCR/fragmentations/fragmentations.h"

namespace XS4GCR {

class Webber1993 : public Fragmentation {
 public:
  Webber1993();
  void init();
  void print() const override;
  std::shared_ptr<Fragmentation> clone() override;
  bool hasChannel(const FragmentationChannel& ch) const override;
  double getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n, bool do_ghosts = false) override;

 protected:
  double direct(const FragmentationChannel& ch, const double& T_n) const;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_WEBBER1993_H_
