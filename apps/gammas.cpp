#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

void get_secondary_production(Pi0GammaModels model, double T_proj, std::string filename) {
  XSECS xsec;
  xsec.setPi0Gammas(model);
  auto x_ap = xsec.createPi0Gammas();

  std::ofstream outfile(filename.c_str());
  outfile << "#T_gamma [GeV] - sigma_pp - sigma_pHe - sigma_Hep - sigma_HeHe [mbarn/GeV]\n";
  outfile << std::scientific;

  const double units = cgs::mbarn / cgs::GeV;

  auto T_pos = UTILS::LogAxis(T_proj / 1e4, T_proj, 1000);
  for (auto T : T_pos) {
    auto sigma_pp = x_ap->get(H1, TARGET::H, T_proj, T);
    auto sigma_phe = x_ap->get(H1, TARGET::He, T_proj, T);
    auto sigma_hep = x_ap->get(He4, TARGET::H, T_proj, T);
    auto sigma_hehe = x_ap->get(He4, TARGET::He, T_proj, T);
    outfile << T / cgs::GeV << "\t";
    outfile << sigma_pp / units << "\t";
    outfile << sigma_phe / units << "\t";
    outfile << sigma_hep / units << "\t";
    outfile << sigma_hehe / units << "\n";
  }
  outfile.close();
}

int main() {
  try {
    LOG::startup_information();
    {
      double T_proj = 1. * cgs::TeV;
      get_secondary_production(Pi0GammaModels::KAMAE2006, T_proj, "output/Kamae2006_1TeV_gamma.txt");
      get_secondary_production(Pi0GammaModels::AAFRAG, T_proj, "output/AAFRAG_1TeV_gamma.txt");
      get_secondary_production(Pi0GammaModels::KELNER2006, T_proj, "output/Kelner2006_1TeV_gamma.txt");
      get_secondary_production(Pi0GammaModels::KAFEXHIU2014GEANT4, T_proj, "output/Kafexhiu2014G4_1TeV_gamma.txt");
      get_secondary_production(Pi0GammaModels::KAFEXHIU2014PYTHIA8, T_proj, "output/Kafexhiu2014P8_1TeV_gamma.txt");
      get_secondary_production(Pi0GammaModels::KAFEXHIU2014SIBYLL, T_proj, "output/Kafexhiu2014Sibyll_1TeV_gamma.txt");
    }
    // {
    //   double T_proj = 100. * cgs::GeV;
    //   get_secondary_production(Pi0GammaModels::KAMAE2006, T_proj, "output/Kamae2006_100GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KELNER2006, T_proj, "output/Kelner2006_100GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::AAFRAG, T_proj, "output/AAFRAG_100GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014GEANT4, T_proj, "output/Kafexhiu2014G4_100GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014PYTHIA8, T_proj,
    //   "output/Kafexhiu2014P8_100GeV_gamma.txt"); get_secondary_production(Pi0GammaModels::KAFEXHIU2014SIBYLL, T_proj,
    //                            "output/Kafexhiu2014Sibyll_100GeV_gamma.txt");
    // }
    // {
    //   double T_proj = 10. * cgs::GeV;
    //   get_secondary_production(Pi0GammaModels::KAMAE2006, T_proj, "output/Kamae2006_10GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KELNER2006, T_proj, "output/Kelner2006_10GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::AAFRAG, T_proj, "output/AAFRAG_10GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014GEANT4, T_proj, "output/Kafexhiu2014G4_10GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014PYTHIA8, T_proj, "output/Kafexhiu2014P8_10GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014SIBYLL, T_proj,
    //   "output/Kafexhiu2014Sibyll_10GeV_gamma.txt");
    // }
    // {
    //   double T_proj = 5. * cgs::GeV;
    //   get_secondary_production(Pi0GammaModels::KAMAE2006, T_proj, "output/Kamae2006_5GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KELNER2006, T_proj, "output/Kelner2006_5GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::AAFRAG, T_proj, "output/AAFRAG_5GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014GEANT4, T_proj, "output/Kafexhiu2014G4_5GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014PYTHIA8, T_proj, "output/Kafexhiu2014P8_5GeV_gamma.txt");
    //   get_secondary_production(Pi0GammaModels::KAFEXHIU2014SIBYLL, T_proj,
    //   "output/Kafexhiu2014Sibyll_5GeV_gamma.txt");
    // }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}