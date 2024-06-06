#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

std::string make_filename(const std::string& filename, const std::string& ytype) {
  auto ofile = "output/" + filename;
  ofile += "_" + ytype;
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
      outfile << xs->get(H1, TARGET::H, T_p, x) / units << "\t";
      outfile << xs->get(H1, TARGET::He, T_p, x) / units << "\t";
      outfile << xs->get(He4, TARGET::H, T_p, x) / units << "\t";
      outfile << xs->get(He4, TARGET::He, T_p, x) / units << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

template <typename T>
double compute_source(std::shared_ptr<T> xs, double slope, double E_s) {
  assert(slope > 2.);
  const auto E_0 = cgs::GeV;
  auto q_s = GSL::gslQAGIntegration<double>(
      [&](double logy) {
        const double y = std::exp(logy);
        const double E_p = y * E_0;
        const double x = E_s / E_p;
        return std::pow(y, -slope) * xs->get(H1, TARGET::H, E_p, x);
      },
      std::log(E_s / E_0), std::log(1e4 * E_s / E_0), 1000, 1e-4);
  return std::pow(E_s / cgs::GeV, slope) * q_s;
}

template <typename T>
void print_secondary_source(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#E_s [GeV] - q(2.2) - q(2.4) - q(2.8)\n";
  outfile << std::scientific;
  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 4 * 16);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    std::cout << E_s / cgs::GeV << "\n";
    outfile << compute_source(xs, 2.2, E_s) / cgs::mbarn << "\t";
    outfile << compute_source(xs, 2.4, E_s) / cgs::mbarn << "\t";
    outfile << compute_source(xs, 2.8, E_s) / cgs::mbarn << "\t";
    outfile << "\n";
  }
  outfile.close();
}

template <typename T>
double compute_probability(std::shared_ptr<T> xs, double slope, double E_s, double E_max) {
  assert(slope > 2.);
  assert(E_max >= E_s);
  if (E_max == E_s) return 0.;
  const auto E_0 = cgs::GeV;
  auto P = GSL::gslQAGIntegration<double>(
      [&](double lny) {
        const auto y = std::exp(lny);
        const double E_p = y * E_0;
        const double x = E_s / E_p;
        return std::pow(y, -slope) * xs->get(H1, TARGET::H, E_p, x);
      },
      std::log(E_s / E_0), std::log(E_max / E_0), 1000, 1e-4);
  return P;
}

template <typename T>
double compute_primary_median_energy(std::shared_ptr<T> xs, double slope, double E_s) {
  const auto Es_max = 1e5 * E_s;
  const auto y_max = compute_probability(xs, slope, E_s, Es_max);
  auto E_p = GSL::rootFinder<double>(
      [&](double x) {
        auto y = compute_probability(xs, slope, E_s, x);
        return y / y_max - 0.5;
      },
      E_s, Es_max, 1000, 0.01);
  return E_p;
}

template <typename T>
void print_primary_energy(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#E_s [GeV] - Ep(2.2) - Ep(2.4) - Ep(2.8)\n";
  outfile << std::scientific;
  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 4 * 16);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    std::cout << E_s / cgs::GeV << "\n";
    outfile << compute_primary_median_energy(xs, 2.2, E_s) / cgs::GeV << "\t";
    outfile << compute_primary_median_energy(xs, 2.4, E_s) / cgs::GeV << "\t";
    outfile << compute_primary_median_energy(xs, 2.8, E_s) / cgs::GeV << "\t";
    outfile << "\n";
  }
  outfile.close();
}

auto neutralParticleType = NeutralParticleType::GAMMA;

void print_gamma_xsecs(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_ph = xsecs.createPi0Gammas(neutralParticleType);
  print_secondary_xsecs<Pi0Gammas>(xs_ph, make_filename(filename, "xsecs"));
  print_secondary_source<Pi0Gammas>(xs_ph, make_filename(filename, "source"));
  //  print_primary_energy<Pi0Gammas>(xs_ph, make_filename(filename, "primary"));
}

void print_ap_xsecs(SecondaryAntiprotonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs_ap = xsecs.createSecondaryAntiprotons();
  print_secondary_xsecs<SecondaryAntiprotons>(xs_ap, make_filename(filename, "xsecs"));
  print_secondary_source<SecondaryAntiprotons>(xs_ap, make_filename(filename, "source"));
}

void print_pos_xsecs(SecondaryLeptonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryLeptons(model);
  auto xs_pos = xsecs.createSecondaryLeptons(positron);
  print_secondary_xsecs<SecondaryLeptons>(xs_pos, make_filename(filename, "xsecs"));
  print_secondary_source<SecondaryLeptons>(xs_pos, make_filename(filename, "source"));
}

void print_terp_xsecs(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_pter = xsecs.createTertiaryProtons();
  print_secondary_xsecs<TertiaryProtons>(xs_pter, make_filename(filename, "xsecs"));
  print_secondary_source<TertiaryProtons>(xs_pter, make_filename(filename, "source"));
}

int main() {
  try {
    LOG::startup_information();
    {
      neutralParticleType = NeutralParticleType::GAMMA;
      print_gamma_xsecs(Pi0GammaModels::KAMAE2006, "Kamae2006_gammas");
      print_gamma_xsecs(Pi0GammaModels::KELNER2006, "Kelner2006_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_gammas");
      print_gamma_xsecs(Pi0GammaModels::AAFRAG, "AAFRAG_gammas");
    }
    {
      neutralParticleType = NeutralParticleType::ALLNUS;
      print_gamma_xsecs(Pi0GammaModels::KAMAE2006, "Kamae2006_nus");
      print_gamma_xsecs(Pi0GammaModels::KELNER2006, "Kelner2006_nus");
      print_gamma_xsecs(Pi0GammaModels::AAFRAG, "AAFRAG_nus");
    }
    {
      print_ap_xsecs(SecondaryAntiprotonModels::AAFRAG, "AAFRAG_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::TANNG1983, "TanNg1983_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::DUPERRAY2003, "Duperray2003_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::DIMAURO2014, "DiMauro2014_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::KORSMEIER2018, "Korsmeier2018_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::WINKLER2017, "Winkler2017_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::FENG2016EPOS, "Feng2016_EPOS_pbar");
      print_ap_xsecs(SecondaryAntiprotonModels::FENG2016QGSJET, "Feng2016_QGSJET_pbar");
    }
    {
      print_pos_xsecs(SecondaryLeptonModels::KAMAE2006, "Kamae2006_pos");
      print_pos_xsecs(SecondaryLeptonModels::AAFRAG, "AAFRAG_pos");
      print_pos_xsecs(SecondaryLeptonModels::HUANGPOHL2007, "HuangPohl2007_pos");
      print_pos_xsecs(SecondaryLeptonModels::ORUSA2022, "Orusa2022_pos");
    }
    { print_terp_xsecs(TertiaryProtonModels::AAFRAG, "AAFRAG_terp"); }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}