#ifndef INCLUDE_XS4GCR_XS4GCR_H
#define INCLUDE_XS4GCR_XS4GCR_H

#include <memory>

#include "XS4GCR/antiprotons/AAfrag.h"
#include "XS4GCR/antiprotons/DiMauro2014.h"
#include "XS4GCR/antiprotons/Duperray2003.h"
#include "XS4GCR/antiprotons/Feng2016.h"
#include "XS4GCR/antiprotons/TanNg1983.h"
#include "XS4GCR/antiprotons/Winkler2017.h"
#include "XS4GCR/inelastic/CROSEC.h"
#include "XS4GCR/inelastic/Letaw1983.h"
#include "XS4GCR/inelastic/Tripathi1999.h"

// #include "XS4GCR/cr_chart.h"
// #include "XS4GCR/leptons/leptons.h"
// #include "XS4GCR/particlenames.h"
// #include "XS4GCR/protons.h"
// #include "XS4GCR/spallation/Evoli2019.h"
// #include "XS4GCR/spallation/Webber1993.h"
// #include "XS4GCR/spallation/spallation.h"

namespace XS4GCR {

enum SecondaryAntiprotonsModels {
  DIMAURO2014,
  DUPERRAY2003,
  TANNG1983,
  WINKLER2017,
  FENG2016QGSJET,
  FENG2016EPOS,
  AAFRAG
};

enum TotalInelasticModels { TRIPATHI1999, CROSEC, LETAW1983 };

class XSECS {
 public:
  XSECS() {}
  virtual ~XSECS() = default;

  std::shared_ptr<TotalInelastic> createTotalInelastic();
  //   std::shared_ptr<Proton_Xsecs> create_proton_xsecs();
  std::shared_ptr<SecondaryAntiprotons> createSecondaryAntiprotons();
  //   std::shared_ptr<Secondary_Leptons> create_secondary_leptons(PID lepton = positron);
  //   std::shared_ptr<CosmicRayChart> create_decay_chart();
  //   std::shared_ptr<Spallation> create_secondary_nuclei();

  inline void setTotalInelastic(TotalInelasticModels model) { totalInelasticModel = model; }

  //   inline void set_proton_xsecs(const std::string &model_name) { proton_xsecs_model = model_name; }

  inline void setSecondaryAntiprotons(SecondaryAntiprotonsModels model) { secondaryAntiprotonsModel = model; }

  //   inline void set_secondary_leptons(const std::string &model_name) { secondary_leptons_model = model_name; }

  //   inline void set_secondary_nuclei(const std::string &model_name) { secondary_nuclei_model = model_name; }

 private:
  TotalInelasticModels totalInelasticModel = TRIPATHI1999;
  SecondaryAntiprotonsModels secondaryAntiprotonsModel = DIMAURO2014;
  //   std::string proton_xsecs_model = "DRAGON2";
  //   std::string secondary_leptons_model = "Kamae2006";
  //   std::string secondary_nuclei_model = "Webber1993";

  std::shared_ptr<TotalInelastic> totalInelastic;
  //   std::shared_ptr<Proton_Xsecs> proton_xsecs;
  //   std::shared_ptr<Secondary_Leptons> secondary_leptons;
  std::shared_ptr<SecondaryAntiprotons> secondaryAntiprotons;
  //   std::shared_ptr<CosmicRayChart> decay_chart;
  //   std::shared_ptr<Spallation> secondary_nuclei;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_XS4GCR_H_
