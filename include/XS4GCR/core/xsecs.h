#ifndef INCLUDE_XS4GCR_XS4GCR_H
#define INCLUDE_XS4GCR_XS4GCR_H

#include <memory>

#include "XS4GCR/antiprotons/SecondaryAntiprotons.h"
#include "XS4GCR/core/nucleiChart.h"
#include "XS4GCR/fragmentations/fragmentations.h"
#include "XS4GCR/inelastic/TotalInelastic.h"
#include "XS4GCR/leptons/SecondaryLeptons.h"
#include "XS4GCR/neutrals/SecondaryNeutrals.h"
#include "XS4GCR/nuclei/SecondaryNuclei.h"
#include "XS4GCR/tertiary/TertiaryProtons.h"

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

enum class SecondaryNeutralModels {
  KAMAE2006,
  KELNER2006,
  KAFEXHIU2014GEANT4,
  KAFEXHIU2014PYTHIA8,
  KAFEXHIU2014SIBYLL,
  AAFRAG
};

using Pi0GammaModels = SecondaryNeutralModels;

enum class SecondaryNucleiModels { AAFRAG };

using TertiaryProtonModels = SecondaryNucleiModels;

enum class TotalInelasticModels { TRIPATHI1999, CROSEC, LETAW1983, GLAUBER };

enum class FragmentationModels {
  FLUKA4DRAGON,
  WEBBER1993,
  USINEGALPROP17OPT12,
  USINEGALPROP17OPT22,
  USINEWEBBER03COSTE12
};

class XSECS {
 public:
  XSECS() {}
  virtual ~XSECS() = default;

  std::shared_ptr<TotalInelastic> createTotalInelastic();
  std::shared_ptr<SecondaryAntiprotons> createSecondaryAntiprotons();
  std::shared_ptr<SecondaryLeptons> createSecondaryLeptons(PID lepton = positron);
  std::shared_ptr<SecondaryNeutrals> createSecondaryNeutrals(NeutralParticleType type = NeutralParticleType::GAMMA);
  std::shared_ptr<Pi0Gammas> createPi0Gammas(NeutralParticleType type = NeutralParticleType::GAMMA);
  std::shared_ptr<TertiaryProtons> createTertiaryProtons();
  std::shared_ptr<SecondaryNuclei> createSecondaryNuclei(NucleusSpecies species = NucleusSpecies::ANTIDEUTERON);
  std::shared_ptr<NucleiChart> createNucleiChart();
  std::shared_ptr<Fragmentation> createFragmentation();

  inline void setTotalInelastic(TotalInelasticModels model) { totalInelasticModel = model; }
  inline void setSecondaryAntiprotons(SecondaryAntiprotonModels model) { secondaryAntiprotonModel = model; }
  inline void setSecondaryLeptons(SecondaryLeptonModels model) { secondaryLeptonModel = model; }
  inline void setSecondaryNeutrals(SecondaryNeutralModels model) { secondaryNeutralModel = model; }
  inline void setPi0Gammas(Pi0GammaModels model) { setSecondaryNeutrals(model); }
  inline void setSecondaryNuclei(SecondaryNucleiModels model) { secondaryNucleiModel = model; }
  inline void setTertiaryProtons(TertiaryProtonModels model) { setSecondaryNuclei(model); }
  inline void setFragmentation(FragmentationModels model) { fragmentationModel = model; }

 private:
  TotalInelasticModels totalInelasticModel = TotalInelasticModels::TRIPATHI1999;
  SecondaryAntiprotonModels secondaryAntiprotonModel = SecondaryAntiprotonModels::DIMAURO2014;
  SecondaryLeptonModels secondaryLeptonModel = SecondaryLeptonModels::KAMAE2006;
  SecondaryNeutralModels secondaryNeutralModel = SecondaryNeutralModels::KAMAE2006;
  SecondaryNucleiModels secondaryNucleiModel = SecondaryNucleiModels::AAFRAG;
  FragmentationModels fragmentationModel = FragmentationModels::FLUKA4DRAGON;

  std::shared_ptr<TotalInelastic> totalInelastic;
  std::shared_ptr<SecondaryLeptons> secondaryLeptons;
  std::shared_ptr<SecondaryAntiprotons> secondaryAntiprotons;
  std::shared_ptr<SecondaryNeutrals> secondaryNeutrals;
  std::shared_ptr<SecondaryNuclei> secondaryNuclei;
  std::shared_ptr<NucleiChart> nucleiChart;
  std::shared_ptr<Fragmentation> fragmentation;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_XS4GCR_H
