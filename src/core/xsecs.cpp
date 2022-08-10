#include "XS4GCR/core/xsecs.h"

#include <memory>

namespace XS4GCR {

std::shared_ptr<TotalInelastic> XSECS::createTotalInelastic() {
  if (totalInelasticModel == TotalInelasticModels::LETAW1983) {
    totalInelastic = std::make_shared<Letaw1983TotalInel>();
  } else if (totalInelasticModel == TotalInelasticModels::TRIPATHI1999) {
    totalInelastic = std::make_shared<Tripathi1999TotalInel>();
  } else if (totalInelasticModel == TotalInelasticModels::CROSEC) {
    totalInelastic = std::make_shared<CrosecTotalInel>();
  } else {
    throw std::invalid_argument("Total inelastic model not found.");
  }
  totalInelastic->print();
  return totalInelastic->clone();
}

std::shared_ptr<SecondaryLeptons> XSECS::createSecondaryLeptons(PID lepton) {
  if (secondaryLeptonsModel == SecondaryLeptonModels::KAMAE2006) {
    secondaryLeptons = std::make_shared<Kamae2006SecLep>(lepton);
  } else if (secondaryLeptonsModel == SecondaryLeptonModels::HUANGPOHL2007) {
    secondaryLeptons = std::make_shared<HuangPohl2007SecLep>(lepton);
  } else {
    throw std::invalid_argument("Secondary Leptons model not found.");
  }
  secondaryLeptons->print();
  return secondaryLeptons->clone();
}

std::shared_ptr<Pi0Gammas> XSECS::createPi0Gammas() {
  if (pi0GammaModel == Pi0GammaModels::KAMAE2006) {
    pi0Gammas = std::make_shared<Kamae2006Gammas>();
  } else if (pi0GammaModel == Pi0GammaModels::KELNER2006) {
    pi0Gammas = std::make_shared<Kelner2006Gammas>();
  } else if (pi0GammaModel == Pi0GammaModels::AAFRAG) {
    pi0Gammas = std::make_shared<AAfragSecGammas>();
  } else {
    throw std::invalid_argument("Secondary Gamma model not found.");
  }
  pi0Gammas->print();
  return pi0Gammas->clone();
}

std::shared_ptr<SecondaryAntiprotons> XSECS::createSecondaryAntiprotons() {
  if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::TANNG1983) {
    secondaryAntiprotons = std::make_shared<TanNg1983SecAp>();
  } else if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::DIMAURO2014) {
    secondaryAntiprotons = std::make_shared<DiMauro2014SecAp>();
  } else if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::DUPERRAY2003) {
    secondaryAntiprotons = std::make_shared<Duperray2003SecAp>();
  } else if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::WINKLER2017) {
    secondaryAntiprotons = std::make_shared<Winkler2017SecAp>();
  } else if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::FENG2016QGSJET) {
    secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_QGSJET04_antiprotons.txt");
  } else if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::FENG2016EPOS) {
    secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_EPOS_LHC_antiprotons.txt");
  } else if (secondaryAntiprotonsModel == SecondaryAntiprotonModels::AAFRAG) {
    secondaryAntiprotons = std::make_shared<AAfragSecAp>();
  } else {
    throw std::invalid_argument("Secondary Antiprotons model not found.");
  }
  secondaryAntiprotons->print();
  return secondaryAntiprotons->clone();
}

// std::shared_ptr<Spallation> XSECS::create_secondary_nuclei() {
//   if (secondary_nuclei_model == "Webber1993") {
//     secondary_nuclei = std::make_shared<Webber1993_Spallation>();
//   } else if (secondary_nuclei_model == "Evoli2019") {
//     secondary_nuclei = std::make_shared<Evoli2019_Spallation>();
//   } else {
//     assert(secondary_nuclei_model == "Webber1993");
//   }
//   secondary_nuclei->print();
//   return secondary_nuclei->clone();
// }

std::shared_ptr<NucleiChart> XSECS::createNucleiChart() {
  nucleiChart = std::make_shared<NucleiChart>();
  nucleiChart->print();
  return nucleiChart->clone();
}

}  // namespace XS4GCR
