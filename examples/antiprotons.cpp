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
  outfile << "#T_ap [GeV] - sigma_pp - sigma_phe - sigma_hep [mbarn]\n";
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

void compute_table(XS4GCR::SecondaryAntiprotonsModels model, std::string filename) {
  XS4GCR::XSECS xsec;
  xsec.setSecondaryAntiprotons(model);
  auto x_ap = xsec.createSecondaryAntiprotons();

  std::ofstream outfile(filename.c_str());
  outfile << "#T_proj [GeV/n] - T_ap [GeV] - sigma_pp - sigma_phe - sigma_hep [mbarn / GeV]\n";
  outfile << std::scientific;

  using XS4GCR::cgs::GeV;
  using XS4GCR::cgs::mbarn;
  using XS4GCR::cgs::TeV;

  const double units = mbarn / GeV;
  auto T_ap = XS4GCR::UTILS::LogAxis(1. * GeV, 10. * TeV, 4 * 32);
  auto T_proj = XS4GCR::UTILS::LogAxis(1. * GeV, 10. * TeV, 4 * 32);
  for (auto T_p : T_proj)
    for (auto T : T_ap) {
      auto sigma_pp = x_ap->get(XS4GCR::H1, XS4GCR::TARGET::H, T_p, T);
      auto sigma_phe = x_ap->get(XS4GCR::H1, XS4GCR::TARGET::He, T_p, T);
      auto sigma_hep = x_ap->get(XS4GCR::He4, XS4GCR::TARGET::H, T_p, T);
      outfile << T_p / GeV << "\t";
      outfile << T / GeV << "\t";
      outfile << sigma_pp / units << "\t" << sigma_phe / units << "\t" << sigma_hep / units << "\n";
    }
  outfile.close();
}

// /**
//  * @brief Compute the non annihilating cross-sections
//  *
//  * This function computes the non annihilating inelastic cross-section of antiprotons
//  * on H in the ISM and write it in a text file, where columns represent: \n
//  * 1) the antiproton kinetic energy in GeV/n \n
//  * 2) \f$\sigma_{in} - \sigma_{ann}\f$ in mbarn
//  *
//  * @param model production model identifier, e.g. Winkler2017
//  * @param filename output file name
//  */
// void get_tertiary_production(std::string model, std::string filename) {
//   XS4GCR::XSECS xsec;
//   xsec.set_secondary_antiprotons(model);
//   auto x_ap = xsec.create_secondary_antiprotons();
//   std::ofstream outfile(filename);
//   outfile << std::scientific;
//   double T_proj = 100 * cgs::GeV;
//   for (double T_ap = 0.1 * cgs::GeV; T_ap < 1e4 * cgs::GeV; T_ap *= 1.1) {
//     double sigma_H = x_ap->get_non_annihilating_inelastic(XS4GCR::H_ISM, T_proj);
//     outfile << T_ap / cgs::GeV << " " << sigma_H / cgs::mbarn << "\n";
//   }
//   outfile.close();
// }

/**
 * @brief Main function for example antiprotons
 */
int main() {
  try {
    XS4GCR::LOG::startup_information();
    {
      double T_proj = 1. * XS4GCR::cgs::TeV;
      //get_secondary_production(XS4GCR::AAFRAG, T_proj, "output/AAFRAG_1TeV_ap.txt");
      get_secondary_production(XS4GCR::TANNG1983, T_proj, "output/TanNg1983_1TeV_ap.txt");
      get_secondary_production(XS4GCR::DUPERRAY2003, T_proj, "output/Duperray2003_1TeV_ap.txt");
      get_secondary_production(XS4GCR::DIMAURO2014, T_proj, "output/DiMauro2014_1TeV_ap.txt");
      get_secondary_production(XS4GCR::WINKLER2017, T_proj, "output/Winkler2017_1TeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_1TeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_1TeV_ap.txt");
    }
    {
      double T_proj = 100. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::TANNG1983, T_proj, "output/TanNg1983_100GeV_ap.txt");
      get_secondary_production(XS4GCR::DUPERRAY2003, T_proj, "output/Duperray2003_100GeV_ap.txt");
      get_secondary_production(XS4GCR::DIMAURO2014, T_proj, "output/DiMauro2014_100GeV_ap.txt");
      get_secondary_production(XS4GCR::WINKLER2017, T_proj, "output/Winkler2017_100GeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_100GeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_100GeV_ap.txt");
    }
    {
      double T_proj = 10. * XS4GCR::cgs::GeV;
      get_secondary_production(XS4GCR::TANNG1983, T_proj, "output/TanNg1983_10GeV_ap.txt");
      get_secondary_production(XS4GCR::DUPERRAY2003, T_proj, "output/Duperray2003_10GeV_ap.txt");
      get_secondary_production(XS4GCR::DIMAURO2014, T_proj, "output/DiMauro2014_10GeV_ap.txt");
      get_secondary_production(XS4GCR::WINKLER2017, T_proj, "output/Winkler2017_10GeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016EPOS, T_proj, "output/Feng2016_EPOS_10GeV_ap.txt");
      get_secondary_production(XS4GCR::FENG2016QGSJET, T_proj, "output/Feng2016_QGSJET_10GeV_ap.txt");
    }

    // get_tertiary_production("Winkler2017", "output/Winkler2017_tertiary.txt");
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
