#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

/**
 * @example antiprotons.cpp
 *
 * In this example the antiproton production cross sections
 * are computed on text files
 *
 * @remark Options available for xsec model : Winkler2017, Feng2016, DiMauro2015, TanNg83
 *
 * @see https://arxiv.org/abs/1711.09616
 */

/**
 * @brief Compute the antiproton production cross-sections.
 *
 * This function computes the ap production cross-sections
 * from the interaction of primary H and He on ISM and write them in a text file,
 * where columns represent: \n
 * 1) the primary kinetic energy per nucleon in GeV/n \n
 * 2) \f$\sigma(p + H_{ISM} \rightarrow \bar{p})\f$ in mbarn \n
 * 3) \f$\sigma(He + H_{ISM} \rightarrow \bar{p})\f$ in mbarn \n
 * 4) \f$\sigma(H + He_{ISM} \rightarrow \bar{p})\f$ in mbarn
 *
 * @param model production model identifier, e.g. Winkler2017
 * @param filename output file name
 */
void get_secondary_production(SecondaryLeptonModels model, double T_proj, std::string filename) {
  XSECS xsec;
  xsec.setSecondaryLeptons(model);
  auto x_ap = xsec.createSecondaryLeptons();

  std::ofstream outfile(filename.c_str());
  outfile << "#T_pos [GeV] - sigma_pp - sigma_phe - sigma_hep [mbarn/GeV]\n";
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
 * @brief Main function for example leptons
 */
int main() {
  try {
    XS4GCR::LOG::startup_information();
    {
      double T_proj = 1. * XS4GCR::cgs::TeV;
      get_secondary_production(XS4GCR::l_KAMAE2006, T_proj, "output/Kamae2006_1TeV_pos.txt");
      get_secondary_production(XS4GCR::HUANGPOHL2007, T_proj, "output/HuangPohl2007_1TeV_pos.txt");
    }
    {
      double T_proj = 100. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::l_KAMAE2006, T_proj, "output/Kamae2006_100GeV_pos.txt");
      get_secondary_production(XS4GCR::HUANGPOHL2007, T_proj, "output/HuangPohl2007_100GeV_pos.txt");
    }
    {
      double T_proj = 10. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::l_KAMAE2006, T_proj, "output/Kamae2006_10GeV_pos.txt");
      get_secondary_production(XS4GCR::HUANGPOHL2007, T_proj, "output/HuangPohl2007_10GeV_pos.txt");
    }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
