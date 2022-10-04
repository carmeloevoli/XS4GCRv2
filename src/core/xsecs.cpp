#include "XS4GCR/core/xsecs.h"

#include <memory>

#include "XS4GCR/antiprotons/AAfragSecAp.h"
#include "XS4GCR/antiprotons/DiMauro2014.h"
#include "XS4GCR/antiprotons/Duperray2003.h"
#include "XS4GCR/antiprotons/Feng2016.h"
#include "XS4GCR/antiprotons/Korsmeier2018.h"
#include "XS4GCR/antiprotons/TanNg1983.h"
#include "XS4GCR/antiprotons/Winkler2017.h"
#include "XS4GCR/core/nucleiChart.h"
#include "XS4GCR/gammas/AAfragSecGammas.h"
#include "XS4GCR/gammas/Kafexhiu2014.h"
#include "XS4GCR/gammas/Kamae2006.h"
#include "XS4GCR/gammas/Kelner2006.h"
#include "XS4GCR/inelastic/CROSEC.h"
#include "XS4GCR/inelastic/Letaw1983.h"
#include "XS4GCR/inelastic/Tripathi1999.h"
#include "XS4GCR/leptons/AAfragSecLeptons.h"
#include "XS4GCR/leptons/HuangPohl2007.h"
#include "XS4GCR/leptons/Kamae2006.h"
#include "XS4GCR/leptons/Orusa2022.h"
#include "XS4GCR/tertiary/AAfragTerProtons.h"

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
  if (secondaryLeptonModel == SecondaryLeptonModels::KAMAE2006) {
    secondaryLeptons = std::make_shared<Kamae2006SecLep>(lepton);
  } else if (secondaryLeptonModel == SecondaryLeptonModels::HUANGPOHL2007) {
    secondaryLeptons = std::make_shared<HuangPohl2007SecLep>(lepton);
  } else if (secondaryLeptonModel == SecondaryLeptonModels::AAFRAG) {
    secondaryLeptons = std::make_shared<AAfragSecLeptons>(lepton);
  } else if (secondaryLeptonModel == SecondaryLeptonModels::ORUSA2022) {
    secondaryLeptons = std::make_shared<Orusa2022SecLep>(lepton);
  } else {
    throw std::invalid_argument("Secondary Leptons model not found.");
  }
  secondaryLeptons->print();
  return secondaryLeptons->clone();
}

std::shared_ptr<Pi0Gammas> XSECS::createPi0Gammas(NeutralParticleType type) {
  if (pi0GammaModel == Pi0GammaModels::KAMAE2006) {
    pi0Gammas = std::make_shared<Kamae2006Gammas>(type);
  } else if (pi0GammaModel == Pi0GammaModels::KELNER2006) {
    pi0Gammas = std::make_shared<Kelner2006Gammas>(type);
  } else if (pi0GammaModel == Pi0GammaModels::KAFEXHIU2014GEANT4) {
    pi0Gammas = std::make_shared<Kafexhiu2014Gammas>(type, InteractionModel::GEANT4);
  } else if (pi0GammaModel == Pi0GammaModels::KAFEXHIU2014PYTHIA8) {
    pi0Gammas = std::make_shared<Kafexhiu2014Gammas>(type, InteractionModel::PYTHIA8);
  } else if (pi0GammaModel == Pi0GammaModels::KAFEXHIU2014SIBYLL) {
    pi0Gammas = std::make_shared<Kafexhiu2014Gammas>(type, InteractionModel::SIBYLL);
  } else if (pi0GammaModel == Pi0GammaModels::AAFRAG) {
    pi0Gammas = std::make_shared<AAfragSecGammas>(type);
  } else {
    throw std::invalid_argument("Secondary Gamma model not found.");
  }
  pi0Gammas->print();
  return pi0Gammas->clone();
}

std::shared_ptr<SecondaryAntiprotons> XSECS::createSecondaryAntiprotons() {
  if (secondaryAntiprotonModel == SecondaryAntiprotonModels::TANNG1983) {
    secondaryAntiprotons = std::make_shared<TanNg1983SecAp>();
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::DIMAURO2014) {
    secondaryAntiprotons = std::make_shared<DiMauro2014SecAp>();
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::DUPERRAY2003) {
    secondaryAntiprotons = std::make_shared<Duperray2003SecAp>();
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::WINKLER2017) {
    secondaryAntiprotons = std::make_shared<Winkler2017SecAp>();
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::KORSMEIER2018) {
    secondaryAntiprotons = std::make_shared<Korsmeier2018SecAp>();
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::FENG2016QGSJET) {
    secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_QGSJET04_antiprotons.txt");
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::FENG2016EPOS) {
    secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_EPOS_LHC_antiprotons.txt");
  } else if (secondaryAntiprotonModel == SecondaryAntiprotonModels::AAFRAG) {
    secondaryAntiprotons = std::make_shared<AAfragSecAp>();
  } else {
    throw std::invalid_argument("Secondary Antiprotons model not found.");
  }
  secondaryAntiprotons->print();
  return secondaryAntiprotons->clone();
}

std::shared_ptr<TertiaryProtons> XSECS::createTertiaryProtons() {
  if (tertiaryProtonModel == TertiaryProtonModels::AAFRAG) {
    tertiaryProtons = std::make_shared<AAfragTerProtons>();
  } else {
    throw std::invalid_argument("Tertiary Protons model not found.");
  }
  tertiaryProtons->print();
  return tertiaryProtons->clone();
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
