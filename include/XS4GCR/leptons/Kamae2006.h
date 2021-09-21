#ifndef INCLUDE_XS4GCR_LEPTONS_KAMAE2006_H
#define INCLUDE_XS4GCR_LEPTONS_KAMAE2006_H

#include "XS4GCR/leptons/SecondaryLeptons.h"

namespace XS4GCR {

// double HeFunction(double T_proton_GeV, double T_lepton_GeV);

class Kamae2006SecLep : public SecondaryLeptons {
 public:
  Kamae2006SecLep(const PID &lepton) { m_lepton = lepton; }

  void print() const override;

  std::shared_ptr<SecondaryLeptons> clone() override;

  double get(const PID &projectile, const TARGET &target, const double &T_proj, const double &T_lepton) const override;

 protected:
  const double HeCorrection = std::pow(4., 2.2 / 3.);
  const double HeHeCorrection = std::pow(4. * 4., 2.2 / 3.);
};

}  // namespace XS4GCR

#endif