#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

std::string make_filename(const std::string& filename) {
  auto ofile = "output/" + filename;
  ofile += ".txt";
  return ofile;
}

template <typename T>
void print_secondary_xsecs(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#T_sec [GeV] - sigma_pp - sigma_pHe - sigma_Hep - sigma_HeHe [mbarn]\n";
  outfile << std::scientific;

  const double units = cgs::mbarn;

  auto xAxis = UTILS::LogAxis(1e-5, 1., 100);
  std::vector<double> TprojAxis = {5. * cgs::GeV, 20. * cgs::GeV, 100. * cgs::GeV, 1. * cgs::TeV, 10. * cgs::TeV};
  for (auto& x : xAxis) {
    outfile << x << "\t";
    for (auto& T_p : TprojAxis) {
      outfile << T_p * xs->get(H1, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs->get(H1, TARGET::He, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs->get(He4, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs->get(He4, TARGET::He, T_p, x * T_p) / units << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

auto neutralParticleType = NeutralParticleType::GAMMA;

void print_gamma_xsecs(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_ph = xsecs.createPi0Gammas(NeutralParticleType::GAMMA);
  print_secondary_xsecs<Pi0Gammas>(xs_ph, make_filename(filename));
}

void print_ap_xsecs(SecondaryAntiprotonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs_ap = xsecs.createSecondaryAntiprotons();
  print_secondary_xsecs<SecondaryAntiprotons>(xs_ap, make_filename(filename));
}

void print_pos_xsecs(SecondaryLeptonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryLeptons(model);
  auto xs_pos = xsecs.createSecondaryLeptons(positron);
  print_secondary_xsecs<SecondaryLeptons>(xs_pos, make_filename(filename));
}

void print_pter_xsecs(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_pter = xsecs.createTertiaryProtons();
  print_secondary_xsecs<TertiaryProtons>(xs_pter, make_filename(filename));
}

int main() {
  try {
    LOG::startup_information();
    {
      neutralParticleType = NeutralParticleType::GAMMA;
      print_gamma_xsecs(Pi0GammaModels::AAFRAG, "AAFRAG_xsecs_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAMAE2006, "Kamae2006_xsecs_gammas");
      print_gamma_xsecs(Pi0GammaModels::KELNER2006, "Kelner2006_xsecs_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_xsecs_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_xsecs_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_xsecs_gammas");
    }
    {
      neutralParticleType = NeutralParticleType::ALLNUS;
      print_gamma_xsecs(Pi0GammaModels::AAFRAG, "AAFRAG_xsecs_nus");
      print_gamma_xsecs(Pi0GammaModels::KAMAE2006, "Kamae2006_xsecs_nus");
      print_gamma_xsecs(Pi0GammaModels::KELNER2006, "Kelner2006_xsecs_nus");
    }
    {
      print_ap_xsecs(SecondaryAntiprotonModels::AAFRAG, "AAFRAG_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::TANNG1983, "TanNg1983_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::DUPERRAY2003, "Duperray2003_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::DIMAURO2014, "DiMauro2014_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::KORSMEIER2018, "Korsmeier2018_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::WINKLER2017, "Winkler2017_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::FENG2016EPOS, "Feng2016_EPOS_xsecs_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::FENG2016QGSJET, "Feng2016_QGSJET_xsecs_pbar");
    }
    {
      print_pos_xsecs(SecondaryLeptonModels::KAMAE2006, "Kamae2006_xsecs_pos");
      print_pos_xsecs(SecondaryLeptonModels::AAFRAG, "AAFRAG_xsecs_pos");
      print_pos_xsecs(SecondaryLeptonModels::HUANGPOHL2007, "HuangPohl2007_xsecs_pos");
      print_pos_xsecs(SecondaryLeptonModels::ORUSA2022, "Orusa2022_xsecs_pos");
    }
    { print_pter_xsecs(TertiaryProtonModels::AAFRAG, "AAFRAG_xsecs_ter"); }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}