#include <fstream>

#include "XS4GCR.h"

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
void get_secondary_production(XS4GCR::SecondaryAntiprotonsModels model, double T_proj, std::string filename) {
  XS4GCR::XSECS xsec;
  xsec.setSecondaryAntiprotons(model);
  auto x_ap = xsec.createSecondaryAntiprotons();

  std::ofstream outfile(filename.c_str());
  outfile << "#T_ap [GeV] - sigma_pp - sigma_phe - sigma_hep [mbarn/GeV]\n";
  outfile << std::scientific;

  using XS4GCR::cgs::GeV;
  using XS4GCR::cgs::mbarn;
  const double units = mbarn / GeV;

  auto T_ap = XS4GCR::UTILS::LogAxis(0.1 * GeV, T_proj, 100);
  for (auto T : T_ap) {
    auto sigma_pp = x_ap->get(XS4GCR::H1, XS4GCR::TARGET::H, T_proj, T);
    auto sigma_phe = x_ap->get(XS4GCR::H1, XS4GCR::TARGET::He, T_proj, T);
    auto sigma_hep = x_ap->get(XS4GCR::He4, XS4GCR::TARGET::H, T_proj, T);
    outfile << T / GeV << "\t";
    outfile << sigma_pp / units << "\t" << sigma_phe / units << "\t" << sigma_hep / units << "\n";
  }
  outfile.close();
}

/**
 * @brief Compute the non annihilating cross-sections
 *
 * This function computes the non annihilating inelastic cross-section of antiprotons
 * on H in the ISM and write it in a text file, where columns represent: \n
 * 1) the antiproton kinetic energy in GeV/n \n
 * 2) \f$\sigma_{in} - \sigma_{ann}\f$ in mbarn
 *
 * @param model production model identifier, e.g. Winkler2017
 * @param filename output file name
 */
void get_tertiary_production(XS4GCR::SecondaryAntiprotonsModels model, double T_proj, std::string filename) {
  XS4GCR::XSECS xsec;
  xsec.setSecondaryAntiprotons(model);
  auto x_ap = xsec.createSecondaryAntiprotons();

  std::ofstream outfile(filename.c_str());
  outfile << "#\n";
  outfile << std::scientific;

  using XS4GCR::cgs::GeV;
  using XS4GCR::cgs::mbarn;
  const double units = mbarn / GeV;

  for (double T_ap = 0.1 * GeV; T_ap < 1e4 * GeV; T_ap *= 1.1) {
    auto sigma_ter = x_ap->getNonAnnihilatingInelastic(XS4GCR::TARGET::H, T_proj);
    outfile << T_ap / GeV << "\t" << sigma_ter / mbarn << "\n";
  }
  outfile.close();
}

/**
 * @brief Main function for example antiprotons
 */
int main() {
  try {
    XS4GCR::LOG::startup_information();
    {
      double T_proj = 1. * XS4GCR::cgs::TeV;
      get_secondary_production(XS4GCR::AAFRAG, T_proj, "output/AAFRAG_1TeV_ap.txt");
      get_secondary_production(XS4GCR::TANNG1983, T_proj, "output/TanNg1983_1TeV_ap.txt");
      get_secondary_production(XS4GCR::DUPERRAY2003, T_proj, "output/Duperray2003_1TeV_ap.txt");
      get_secondary_production(XS4GCR::DIMAURO2014, T_proj, "output/DiMauro2014_1TeV_ap.txt");
      get_secondary_production(XS4GCR::WINKLER2017, T_proj, "output/Winkler2017_1TeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_1TeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_1TeV_ap.txt");
    }
    {
      double T_proj = 100. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::AAFRAG, T_proj, "output/AAFRAG_100GeV_ap.txt");
      get_secondary_production(XS4GCR::TANNG1983, T_proj, "output/TanNg1983_100GeV_ap.txt");
      get_secondary_production(XS4GCR::DUPERRAY2003, T_proj, "output/Duperray2003_100GeV_ap.txt");
      get_secondary_production(XS4GCR::DIMAURO2014, T_proj, "output/DiMauro2014_100GeV_ap.txt");
      get_secondary_production(XS4GCR::WINKLER2017, T_proj, "output/Winkler2017_100GeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_100GeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_100GeV_ap.txt");
    }
    // {
    //   double T_proj = 10. * XS4GCR::cgs::GeV;
    //   get_secondary_production(XS4GCR::TANNG1983, T_proj, "output/TanNg1983_10GeV_ap.txt");
    //   get_secondary_production(XS4GCR::DUPERRAY2003, T_proj, "output/Duperray2003_10GeV_ap.txt");
    //   get_secondary_production(XS4GCR::DIMAURO2014, T_proj, "output/DiMauro2014_10GeV_ap.txt");
    //   get_secondary_production(XS4GCR::WINKLER2017, T_proj, "output/Winkler2017_10GeV_ap.txt");
    //   get_secondary_production(XS4GCR::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_10GeV_ap.txt");
    //   get_secondary_production(XS4GCR::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_10GeV_ap.txt");
    // }
    // {
    //   double T_proj = 100. * XS4GCR::cgs::GeV;
    //   get_tertiary_production(XS4GCR::TANNG1983, T_proj, "output/tertiary_ap_100GeV.txt");
    // }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
