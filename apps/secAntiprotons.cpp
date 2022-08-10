#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

void get_secondary_production(SecondaryAntiprotonModels model, double T_proj, std::string filename) {
  XSECS xsec;
  xsec.setSecondaryAntiprotons(model);
  auto x_ap = xsec.createSecondaryAntiprotons();

  std::ofstream outfile(filename.c_str());
  outfile << "#T_ap [GeV] - sigma_pp - sigma_phe - sigma_hep [mbarn/GeV]\n";
  outfile << std::scientific;

  const double units = cgs::mbarn / cgs::GeV;

  auto T_ap = UTILS::LogAxis(0.1 * cgs::GeV, T_proj, 1000);
  for (auto T : T_ap) {
    auto sigma_pp = x_ap->get(H1, TARGET::H, T_proj, T);
    auto sigma_phe = x_ap->get(H1, TARGET::He, T_proj, T);
    auto sigma_hep = x_ap->get(He4, TARGET::H, T_proj, T);
    outfile << T / cgs::GeV << "\t";
    outfile << sigma_pp / units << "\t" << sigma_phe / units << "\t" << sigma_hep / units << "\n";
  }
  outfile.close();
}

int main() {
  try {
    LOG::startup_information();
    {
      const double T_proj = 1. * cgs::TeV;
      get_secondary_production(SecondaryAntiprotonModels::AAFRAG, T_proj, "output/AAFRAG_1TeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::TANNG1983, T_proj, "output/TanNg1983_1TeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::DUPERRAY2003, T_proj, "output/Duperray2003_1TeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::DIMAURO2014, T_proj, "output/DiMauro2014_1TeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::WINKLER2017, T_proj, "output/Winkler2017_1TeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_1TeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_1TeV_ap.txt");
    }
    {
      const double T_proj = 100. * cgs::GeV;
      get_secondary_production(SecondaryAntiprotonModels::AAFRAG, T_proj, "output/AAFRAG_100GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::TANNG1983, T_proj, "output/TanNg1983_100GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::DUPERRAY2003, T_proj, "output/Duperray2003_100GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::DIMAURO2014, T_proj, "output/DiMauro2014_100GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::WINKLER2017, T_proj, "output/Winkler2017_100GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_100GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::FENG2016QGSJET, T_proj,
                               "output/Feng2016_QGSJET_100GeV_ap.txt");
    }
    {
      const double T_proj = 10. * cgs::GeV;
      get_secondary_production(SecondaryAntiprotonModels::AAFRAG, T_proj, "output/AAFRAG_10GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::TANNG1983, T_proj, "output/TanNg1983_10GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::DUPERRAY2003, T_proj, "output/Duperray2003_10GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::DIMAURO2014, T_proj, "output/DiMauro2014_10GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::WINKLER2017, T_proj, "output/Winkler2017_10GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_10GeV_ap.txt");
      get_secondary_production(SecondaryAntiprotonModels::FENG2016QGSJET, T_proj,
                               "output/Feng2016_QGSJET_10GeV_ap.txt");
    }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
