#include "XS4GCR/XS4GCR.h"

#include <memory>

namespace XS4GCR {

// std::shared_ptr<Total_Inelastic> XSECS::create_total_inelastic() {
//     if (total_inelastic_model == "Letaw83") {
//         total_inelastic = std::make_shared<Letaw1983_Total_Inelastic>();
//     } else if (total_inelastic_model == "Tripathi99") {
//         total_inelastic = std::make_shared<Tripathi99_Total_Inelastic>();
//     } else if (total_inelastic_model == "CROSEC") {
//         total_inelastic = std::make_shared<CROSEC_Total_Inelastic>();
//     } else {
//         return nullptr;
//     }
//     total_inelastic->print();
//     return total_inelastic->clone();
// }

// std::shared_ptr<Proton_Xsecs> XSECS::create_proton_xsecs() {
//     if (proton_xsecs_model == "DRAGON2") {
//         proton_xsecs = std::make_shared<DRAGON2_Proton_Xsecs>();
//     } else {
//         assert(proton_xsecs_model == "DRAGON2");
//     }
//     proton_xsecs->print();
//     return proton_xsecs->clone();
// }

// std::shared_ptr<Secondary_Leptons> XSECS::create_secondary_leptons(PID lepton) {
//     if (secondary_leptons_model == "Kamae2006") {
//         secondary_leptons = std::make_shared<Kamae2006_Secondary_Leptons>(lepton);
//     } else if (secondary_leptons_model == "HuangPohl2007") {
//         secondary_leptons = std::make_shared<HuangPohl2007_Secondary_Leptons>(lepton);
//     } else {
//         assert(secondary_leptons_model == "Kamae2006");
//         return 0;
//     }
//     secondary_leptons->print();
//     return secondary_leptons->clone();
// }

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
    LOGE << "Secondary Antiprotons model not found.";
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
