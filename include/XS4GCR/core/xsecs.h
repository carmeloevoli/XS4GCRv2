#ifndef INCLUDE_XS4GCR_XS4GCR_H
#define INCLUDE_XS4GCR_XS4GCR_H

#include <memory>

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"
#include "XS4GCR/core/nucleiChart.h"
#include "XS4GCR/gammas/Pi0Gammas.h"
#include "XS4GCR/inelastic/TotalInelastic.h"
#include "XS4GCR/leptons/SecondaryLeptons.h"
#include "XS4GCR/tertiary/TertiaryProtons.h"

// #include "XS4GCR/antiprotons/AAfragSecAp.h"
// #include "XS4GCR/antiprotons/DiMauro2014.h"
// #include "XS4GCR/antiprotons/Duperray2003.h"
// #include "XS4GCR/antiprotons/Feng2016.h"
// #include "XS4GCR/antiprotons/TanNg1983.h"
// #include "XS4GCR/antiprotons/Winkler2017.h"
// #include "XS4GCR/core/nucleiChart.h"
// #include "XS4GCR/gammas/AAfragSecGammas.h"
// #include "XS4GCR/gammas/Kafexhiu2014.h"
// #include "XS4GCR/gammas/Kamae2006.h"
// #include "XS4GCR/gammas/Kelner2006.h"
// #include "XS4GCR/inelastic/CROSEC.h"
// #include "XS4GCR/inelastic/Letaw1983.h"
// #include "XS4GCR/inelastic/Tripathi1999.h"
// #include "XS4GCR/leptons/AAfragSecLeptons.h"
// #include "XS4GCR/leptons/HuangPohl2007.h"
// #include "XS4GCR/leptons/Kamae2006.h"
// #include "XS4GCR/leptons/Orusa2022.h"

// #include "XS4GCR/particlenames.h"
// #include "XS4GCR/protons.h"
// #include "XS4GCR/spallation/Evoli2019.h"
// #include "XS4GCR/spallation/Webber1993.h"
// #include "XS4GCR/spallation/spallation.h"

namespace XS4GCR {

enum class SecondaryAntiprotonModels {
  DIMAURO2014,
  DUPERRAY2003,
  TANNG1983,
  WINKLER2017,
  KORSMEIER2018,
  FENG2016QGSJET,
  FENG2016EPOS,
  AAFRAG
};

enum class SecondaryLeptonModels { KAMAE2006, HUANGPOHL2007, ORUSA2022, AAFRAG };

enum class Pi0GammaModels {
  KAMAE2006,
  KELNER2006,
  KAFEXHIU2014GEANT4,
  KAFEXHIU2014PYTHIA8,
  KAFEXHIU2014SIBYLL,
  AAFRAG
};

enum class TertiaryProtonModels { AAFRAG };

enum class TotalInelasticModels { TRIPATHI1999, CROSEC, LETAW1983 };

class XSECS {
 public:
  XSECS() {}
  virtual ~XSECS() = default;

  std::shared_ptr<TotalInelastic> createTotalInelastic();
  std::shared_ptr<SecondaryAntiprotons> createSecondaryAntiprotons();
  std::shared_ptr<SecondaryLeptons> createSecondaryLeptons(PID lepton = positron);
  std::shared_ptr<Pi0Gammas> createPi0Gammas(NeutralParticleType type = NeutralParticleType::GAMMA);
  std::shared_ptr<TertiaryProtons> createTertiaryProtons();
  std::shared_ptr<NucleiChart> createNucleiChart();

  //   std::shared_ptr<Spallation> create_secondary_nuclei();

  inline void setTotalInelastic(TotalInelasticModels model) { totalInelasticModel = model; }
  inline void setSecondaryAntiprotons(SecondaryAntiprotonModels model) { secondaryAntiprotonModel = model; }
  inline void setSecondaryLeptons(SecondaryLeptonModels model) { secondaryLeptonModel = model; }
  inline void setPi0Gammas(Pi0GammaModels model) { pi0GammaModel = model; }
  inline void setTertiaryProtons(TertiaryProtonModels model) { tertiaryProtonModel = model; }

  //   inline void set_secondary_nuclei(const std::string &model_name) { secondary_nuclei_model = model_name; }

 private:
  TotalInelasticModels totalInelasticModel = TotalInelasticModels::TRIPATHI1999;
  SecondaryAntiprotonModels secondaryAntiprotonModel = SecondaryAntiprotonModels::DIMAURO2014;
  SecondaryLeptonModels secondaryLeptonModel = SecondaryLeptonModels::KAMAE2006;
  Pi0GammaModels pi0GammaModel = Pi0GammaModels::KAMAE2006;
  TertiaryProtonModels tertiaryProtonModel = TertiaryProtonModels::AAFRAG;

  //   std::string secondary_nuclei_model = "Webber1993";

  std::shared_ptr<TotalInelastic> totalInelastic;
  std::shared_ptr<SecondaryLeptons> secondaryLeptons;
  std::shared_ptr<SecondaryAntiprotons> secondaryAntiprotons;
  std::shared_ptr<Pi0Gammas> pi0Gammas;
  std::shared_ptr<TertiaryProtons> tertiaryProtons;
  std::shared_ptr<NucleiChart> nucleiChart;
  //   std::shared_ptr<Spallation> secondary_nuclei;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_XS4GCR_H
