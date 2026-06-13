#include "XS4GCR/core/xsecs.h"

#include <memory>

#include "XS4GCR/antiprotons/AAfragSecAp.h"
#include "XS4GCR/antiprotons/DiMauro2014.h"
#include "XS4GCR/antiprotons/Duperray2003.h"
#include "XS4GCR/antiprotons/Feng2016.h"
#include "XS4GCR/antiprotons/Korsmeier2018.h"
#include "XS4GCR/antiprotons/TanNg1983.h"
#include "XS4GCR/antiprotons/Winkler2017.h"
#include "XS4GCR/fragmentations/Evoli2026.h"
#include "XS4GCR/fragmentations/Fluka4Dragon.h"
#include "XS4GCR/fragmentations/UsineFragmentationTables.h"
#include "XS4GCR/fragmentations/Webber1993.h"
#include "XS4GCR/inelastic/CROSEC.h"
#include "XS4GCR/inelastic/Glauber.h"
#include "XS4GCR/inelastic/Letaw1983.h"
#include "XS4GCR/inelastic/Tripathi1999.h"
#include "XS4GCR/leptons/AAfragSecLeptons.h"
#include "XS4GCR/leptons/HuangPohl2007.h"
#include "XS4GCR/leptons/Kamae2006.h"
#include "XS4GCR/leptons/Orusa2022.h"
#include "XS4GCR/neutrals/AAfragSecNeutrals.h"
#include "XS4GCR/neutrals/Kafexhiu2014.h"
#include "XS4GCR/neutrals/Kamae2006.h"
#include "XS4GCR/neutrals/Kelner2006.h"
#include "XS4GCR/nuclei/AAfragSecNuclei.h"

namespace XS4GCR {

std::shared_ptr<TotalInelastic> XSECS::createTotalInelastic() {
  switch (totalInelasticModel) {
    case TotalInelasticModels::LETAW1983:
      totalInelastic = std::make_shared<Letaw1983TotalInel>();
      break;
    case TotalInelasticModels::TRIPATHI1999:
      totalInelastic = std::make_shared<Tripathi1999TotalInel>();
      break;
    case TotalInelasticModels::CROSEC:
      totalInelastic = std::make_shared<CrosecTotalInel>();
      break;
    case TotalInelasticModels::GLAUBER:
      totalInelastic = std::make_shared<GlauberTotalInel>();
      break;
    default:
      throw std::invalid_argument("Total inelastic model not found.");
  }
  totalInelastic->print();
  return totalInelastic->clone();
}

std::shared_ptr<SecondaryLeptons> XSECS::createSecondaryLeptons(PID lepton) {
  switch (secondaryLeptonModel) {
    case SecondaryLeptonModels::KAMAE2006:
      secondaryLeptons = std::make_shared<Kamae2006SecLep>(lepton);
      break;
    case SecondaryLeptonModels::HUANGPOHL2007:
      secondaryLeptons = std::make_shared<HuangPohl2007SecLep>(lepton);
      break;
    case SecondaryLeptonModels::AAFRAG:
      secondaryLeptons = std::make_shared<AAfragSecLeptons>(lepton);
      break;
    case SecondaryLeptonModels::ORUSA2022:
      secondaryLeptons = std::make_shared<Orusa2022SecLep>(lepton);
      break;
    default:
      throw std::invalid_argument("Secondary Leptons model not found.");
  }
  secondaryLeptons->print();
  return secondaryLeptons->clone();
}

std::shared_ptr<SecondaryNeutrals> XSECS::createSecondaryNeutrals(NeutralParticleType type) {
  switch (secondaryNeutralModel) {
    case SecondaryNeutralModels::KAMAE2006:
      secondaryNeutrals = std::make_shared<Kamae2006Neutrals>(type);
      break;
    case SecondaryNeutralModels::KELNER2006:
      secondaryNeutrals = std::make_shared<Kelner2006Neutrals>(type);
      break;
    case SecondaryNeutralModels::KAFEXHIU2014GEANT4:
      secondaryNeutrals = std::make_shared<Kafexhiu2014Neutrals>(type, InteractionModel::GEANT4);
      break;
    case SecondaryNeutralModels::KAFEXHIU2014PYTHIA8:
      secondaryNeutrals = std::make_shared<Kafexhiu2014Neutrals>(type, InteractionModel::PYTHIA8);
      break;
    case SecondaryNeutralModels::KAFEXHIU2014SIBYLL:
      secondaryNeutrals = std::make_shared<Kafexhiu2014Neutrals>(type, InteractionModel::SIBYLL);
      break;
    case SecondaryNeutralModels::AAFRAG:
      secondaryNeutrals = std::make_shared<AAfragSecNeutrals>(type);
      break;
    default:
      throw std::invalid_argument("Secondary neutral model not found.");
  }
  secondaryNeutrals->print();
  return secondaryNeutrals->clone();
}

std::shared_ptr<Pi0Gammas> XSECS::createPi0Gammas(NeutralParticleType type) {
  return createSecondaryNeutrals(type);
}

std::shared_ptr<SecondaryAntiprotons> XSECS::createSecondaryAntiprotons() {
  switch (secondaryAntiprotonModel) {
    case SecondaryAntiprotonModels::TANNG1983:
      secondaryAntiprotons = std::make_shared<TanNg1983SecAp>();
      break;
    case SecondaryAntiprotonModels::DIMAURO2014:
      secondaryAntiprotons = std::make_shared<DiMauro2014SecAp>();
      break;
    case SecondaryAntiprotonModels::DUPERRAY2003:
      secondaryAntiprotons = std::make_shared<Duperray2003SecAp>();
      break;
    case SecondaryAntiprotonModels::WINKLER2017:
      secondaryAntiprotons = std::make_shared<Winkler2017SecAp>();
      break;
    case SecondaryAntiprotonModels::KORSMEIER2018:
      secondaryAntiprotons = std::make_shared<Korsmeier2018SecAp>();
      break;
    case SecondaryAntiprotonModels::FENG2016QGSJET:
      secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_QGSJET04_antiprotons.txt");
      break;
    case SecondaryAntiprotonModels::FENG2016EPOS:
      secondaryAntiprotons = std::make_shared<Feng2016SecAp>("data/Feng2016_EPOS_LHC_antiprotons.txt");
      break;
    case SecondaryAntiprotonModels::AAFRAG:
      secondaryAntiprotons = std::make_shared<AAfragSecAp>();
      break;
    default:
      throw std::invalid_argument("Secondary Antiprotons model not found.");
  }
  secondaryAntiprotons->print();
  return secondaryAntiprotons->clone();
}

std::shared_ptr<TertiaryProtons> XSECS::createTertiaryProtons() {
  return createSecondaryNuclei(NucleusSpecies::PROTON);
}

std::shared_ptr<SecondaryNuclei> XSECS::createSecondaryNuclei(NucleusSpecies species) {
  switch (secondaryNucleiModel) {
    case SecondaryNucleiModels::AAFRAG:
      secondaryNuclei = std::make_shared<AAfragSecNuclei>(species);
      break;
    default:
      throw std::invalid_argument("Secondary Nuclei model not found.");
  }
  secondaryNuclei->print();
  return secondaryNuclei->clone();
}

std::shared_ptr<Fragmentation> XSECS::createFragmentation() {
  switch (fragmentationModel) {
    case FragmentationModels::EVOLI2026:
      fragmentation = std::make_shared<Evoli2026>();
      break;
    case FragmentationModels::FLUKA4DRAGON:
      fragmentation = std::make_shared<Fluka4Dragon>();
      break;
    case FragmentationModels::WEBBER1993:
      fragmentation = std::make_shared<Webber1993>();
      break;
    case FragmentationModels::USINEGALPROP17OPT12:
      fragmentation = std::make_shared<UsineFragmentation>("data/usine-v3.5/sigProdGALPROP17_OPT12.dat");
      break;
    case FragmentationModels::USINEGALPROP17OPT22:
      fragmentation = std::make_shared<UsineFragmentation>("data/usine-v3.5/sigProdGALPROP17_OPT22.dat");
      break;
    case FragmentationModels::USINEWEBBER03COSTE12:
      fragmentation = std::make_shared<UsineFragmentation>("data/usine-v3.5/sigProdWebber03+Coste12.dat");
      break;
    default:
      throw std::invalid_argument("Fragmentation model not found.");
  }
  fragmentation->print();
  return fragmentation->clone();
}

}  // namespace XS4GCR
