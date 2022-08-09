#ifndef INCLUDE_XS4GCR_XS4GCR_H
#define INCLUDE_XS4GCR_XS4GCR_H

#include <memory>

#include "XS4GCR/antiprotons/AAfrag.h"
#include "XS4GCR/antiprotons/DiMauro2014.h"
#include "XS4GCR/antiprotons/Duperray2003.h"
#include "XS4GCR/antiprotons/Feng2016.h"
#include "XS4GCR/antiprotons/TanNg1983.h"
#include "XS4GCR/antiprotons/Winkler2017.h"
#include "XS4GCR/core/nucleiChart.h"
#include "XS4GCR/gammas/Kamae2006.h"
#include "XS4GCR/gammas/Kelner2006.h"
#include "XS4GCR/inelastic/CROSEC.h"
#include "XS4GCR/inelastic/Letaw1983.h"
#include "XS4GCR/inelastic/Tripathi1999.h"
#include "XS4GCR/leptons/HuangPohl2007.h"
#include "XS4GCR/leptons/Kamae2006.h"

// #include "XS4GCR/particlenames.h"
// #include "XS4GCR/protons.h"
// #include "XS4GCR/spallation/Evoli2019.h"
// #include "XS4GCR/spallation/Webber1993.h"
// #include "XS4GCR/spallation/spallation.h"

namespace XS4GCR {

enum SecondaryAntiprotonModels {
  DIMAURO2014,
  DUPERRAY2003,
  TANNG1983,
  WINKLER2017,
  FENG2016QGSJET,
  FENG2016EPOS,
  AAFRAG
};

enum SecondaryLeptonModels { l_KAMAE2006, HUANGPOHL2007 };

enum Pi0GammaModels { g_KAMAE2006, g_KELNER2006 };

enum TotalInelasticModels { TRIPATHI1999, CROSEC, LETAW1983 };

class XSECS {
 public:
  XSECS() {}
  virtual ~XSECS() = default;

  std::shared_ptr<TotalInelastic> createTotalInelastic();
  std::shared_ptr<SecondaryAntiprotons> createSecondaryAntiprotons();
  std::shared_ptr<SecondaryLeptons> createSecondaryLeptons(PID lepton = positron);
  std::shared_ptr<Pi0Gammas> createPi0Gammas();
  std::shared_ptr<NucleiChart> createNucleiChart();

  //   std::shared_ptr<Spallation> create_secondary_nuclei();

  inline void setTotalInelastic(TotalInelasticModels model) { totalInelasticModel = model; }
  inline void setSecondaryAntiprotons(SecondaryAntiprotonModels model) { secondaryAntiprotonsModel = model; }
  inline void setSecondaryLeptons(SecondaryLeptonModels model) { secondaryLeptonsModel = model; }
  inline void setPi0Gammas(Pi0GammaModels model) { pi0GammaModel = model; }

  //   inline void set_secondary_nuclei(const std::string &model_name) { secondary_nuclei_model = model_name; }

 private:
  TotalInelasticModels totalInelasticModel = TRIPATHI1999;
  SecondaryAntiprotonModels secondaryAntiprotonsModel = DIMAURO2014;
  SecondaryLeptonModels secondaryLeptonsModel = l_KAMAE2006;
  Pi0GammaModels pi0GammaModel = g_KAMAE2006;

  //   std::string secondary_nuclei_model = "Webber1993";

  std::shared_ptr<TotalInelastic> totalInelastic;
  std::shared_ptr<SecondaryLeptons> secondaryLeptons;
  std::shared_ptr<SecondaryAntiprotons> secondaryAntiprotons;
  std::shared_ptr<Pi0Gammas> pi0Gammas;
  std::shared_ptr<NucleiChart> nucleiChart;
  //   std::shared_ptr<Spallation> secondary_nuclei;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_XS4GCR_H
