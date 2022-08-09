#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

void get_secondary_production(Pi0GammaModels model, double T_proj, std::string filename) {
  XSECS xsec;
  xsec.setPi0Gammas(model);
  auto x_ap = xsec.createPi0Gammas();

  std::ofstream outfile(filename.c_str());
  outfile << "#T_gamma [GeV] - sigma_pp - sigma_phe - sigma_hep [mbarn/GeV]\n";
  outfile << std::scientific;

  const double units = cgs::mbarn / cgs::GeV;

  auto T_pos = UTILS::LogAxis(T_proj / 1e4, T_proj, 1000);
  for (auto T : T_pos) {
    auto sigma_pp = x_ap->get(H1, TARGET::H, T_proj, T);
    auto sigma_phe = x_ap->get(H1, TARGET::He, T_proj, T);
    auto sigma_hep = x_ap->get(He4, TARGET::H, T_proj, T);
    outfile << T / cgs::GeV << "\t";
    outfile << sigma_pp / units << "\t" << sigma_phe / units << "\t" << sigma_hep / units << "\n";
  }
  outfile.close();
}

/**
 * @brief Main function for example gammas
 */
int main() {
  try {
    XS4GCR::LOG::startup_information();
    {
      double T_proj = 1. * XS4GCR::cgs::TeV;
      get_secondary_production(XS4GCR::g_KAMAE2006, T_proj, "output/Kamae2006_1TeV_gamma.txt");
      // get_secondary_production(XS4GCR::HUANGPOHL2007, T_proj, "output/HuangPohl2007_1TeV_pos.txt");
    }
    {
      double T_proj = 100. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::g_KAMAE2006, T_proj, "output/Kamae2006_100GeV_gamma.txt");
      // get_secondary_production(XS4GCR::HUANGPOHL2007, T_proj, "output/HuangPohl2007_100GeV_pos.txt");
    }
    {
      double T_proj = 10. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::g_KAMAE2006, T_proj, "output/Kamae2006_10GeV_gamma.txt");
      // get_secondary_production(XS4GCR::HUANGPOHL2007, T_proj, "output/HuangPohl2007_10GeV_pos.txt");
    }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
