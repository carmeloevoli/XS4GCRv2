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
double compute_source(std::shared_ptr<T> xs, double slope, double E_s) {
  auto q_s = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        double E_p = std::exp(lnEp);
        return std::pow(E_p, 1. - slope) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log(E_s), std::log(1e4 * E_s), 1000, 1e-4);
  return q_s;
}

template <typename T>
void print_secondary_source(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#E_s [GeV] - q(2.0) - q(2.4) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs, 2.0, E_s) << "\t";
    outfile << compute_source(xs, 2.4, E_s) << "\t";
    outfile << compute_source(xs, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

auto neutralParticleType = NeutralParticleType::GAMMA;

void print_gamma_source(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_ph = xsecs.createPi0Gammas(NeutralParticleType::GAMMA);
  print_secondary_source<Pi0Gammas>(xs_ph, make_filename(filename));
}

void print_ap_source(SecondaryAntiprotonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs_ap = xsecs.createSecondaryAntiprotons();
  print_secondary_source<SecondaryAntiprotons>(xs_ap, make_filename(filename));
}

void print_pos_source(SecondaryLeptonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryLeptons(model);
  auto xs_pos = xsecs.createSecondaryLeptons(positron);
  print_secondary_source<SecondaryLeptons>(xs_pos, make_filename(filename));
}

void print_pter_source(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_pter = xsecs.createTertiaryProtons();
  print_secondary_source<TertiaryProtons>(xs_pter, make_filename(filename));
}

int main() {
  try {
    LOG::startup_information();
    {
      neutralParticleType = NeutralParticleType::GAMMA;
      print_gamma_source(Pi0GammaModels::KAMAE2006, "Kamae2006_source_gammas");
      print_gamma_source(Pi0GammaModels::KELNER2006, "Kelner2006_source_gammas");
      print_gamma_source(Pi0GammaModels::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_source_gammas");
      print_gamma_source(Pi0GammaModels::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_source_gammas");
      print_gamma_source(Pi0GammaModels::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_source_gammas");
      print_gamma_source(Pi0GammaModels::AAFRAG, "AAFRAG_source_gammas");
    }
    {
      neutralParticleType = NeutralParticleType::ALLNUS;
      print_gamma_source(Pi0GammaModels::KAMAE2006, "Kamae2006_source_nus");
      print_gamma_source(Pi0GammaModels::KELNER2006, "Kelner2006_source_nus");
      print_gamma_source(Pi0GammaModels::AAFRAG, "AAFRAG_source_nus");
    }
    {
      print_ap_source(SecondaryAntiprotonModels::TANNG1983, "TanNg1983_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::DUPERRAY2003, "Duperray2003_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::DIMAURO2014, "DiMauro2014_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::KORSMEIER2018, "Korsmeier2018_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::WINKLER2017, "Winkler2017_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::FENG2016EPOS, "Feng2016_EPOS_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::FENG2016QGSJET, "Feng2016_QGSJET_source_pbar");
      print_ap_source(SecondaryAntiprotonModels::AAFRAG, "AAFRAG_source_pbar");
    }
    {
      print_pos_source(SecondaryLeptonModels::KAMAE2006, "Kamae2006_source_pos");
      print_pos_source(SecondaryLeptonModels::HUANGPOHL2007, "HuangPohl2007_source_pos");
      print_pos_source(SecondaryLeptonModels::ORUSA2022, "Orusa2022_source_pos");
      print_pos_source(SecondaryLeptonModels::AAFRAG, "AAFRAG_source_pos");
    }
    { print_pter_source(TertiaryProtonModels::AAFRAG, "AAFRAG_source_ter"); }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}