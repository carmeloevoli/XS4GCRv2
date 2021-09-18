#include "XS4GCR/XS4GCR.h"

#include <memory>

namespace XS4GCR {

std::shared_ptr<TotalInelastic> XSECS::createTotalInelastic() {
  if (totalInelasticModel == LETAW1983) {
    totalInelastic = std::make_shared<Letaw1983TotalInel>();
  } else if (totalInelasticModel == TRIPATHI1999) {
    totalInelastic = std::make_shared<Tripathi1999TotalInel>();
  } else if (totalInelasticModel == CROSEC) {
    totalInelastic = std::make_shared<CrosecTotalInel>();
  } else {
    throw std::invalid_argument("Total inelastic model not found.");
  }
  totalInelastic->print();
  return totalInelastic->clone();
}

std::shared_ptr<SecondaryLeptons> XSECS::createSecondaryLeptons(PID lepton) {
  if (secondaryLeptonsModel == KAMAE2006) {
    secondaryLeptons = std::make_shared<Kamae2006SecLep>(lepton);
  } else if (secondaryLeptonsModel == HUANGPOHL2007) {
    secondaryLeptons = std::make_shared<HuangPohl2007SecLep>(lepton);
  } else {
    throw std::invalid_argument("Secondary Leptons model not found.");
  }
  secondaryLeptons->print();
  return secondaryLeptons->clone();
}

std::shared_ptr<SecondaryAntiprotons> XSECS::createSecondaryAntiprotons() {
  if (secondaryAntiprotonsModel == TANNG1983) {
    secondaryAntiprotons = std::make_shared<TanNg1983SecAp>();
  } else if (secondaryAntiprotonsModel == DIMAURO2014) {
    secondaryAntiprotons = std::make_shared<DiMauro2014SecAp>();
  } else if (secondaryAntiprotonsModel == DUPERRAY2003) {
    secondaryAntiprotons = std::make_shared<Duperray2003SecAp>();
  } else if (secondaryAntiprotonsModel == WINKLER2017) {
    secondaryAntiprotons = std::make_shared<Winkler2017SecAp>("data/Winkler2017_antiprotons.txt");
  } else if (secondaryAntiprotonsModel == FENG2016QGSJET) {
    secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_QGSJET04_antiprotons.txt");
  } else if (secondaryAntiprotonsModel == FENG2016EPOS) {
    secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_EPOS_LHC_antiprotons.txt");
  } else if (secondaryAntiprotonsModel == AAFRAG) {
    secondaryAntiprotons = std::make_shared<AAfragSecAp>("data/AAfrag_v101_antiprotons.txt");
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

// std::shared_ptr<CosmicRayChart> XSECS::create_decay_chart() {
//   decay_chart = std::make_shared<DefaultCosmicRayChart>();
//   decay_chart->print();
//   decay_chart->init();
//   return decay_chart->clone();
// }

}  // namespace XS4GCR
