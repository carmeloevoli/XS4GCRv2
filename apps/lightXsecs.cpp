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
      outfile << T_p * xs->get(H1, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs->get(H1, TARGET::He, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs->get(He4, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs->get(He4, TARGET::He, T_p, x * T_p) / units << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

template <typename T>
double compute_source(std::shared_ptr<T> xs, double slope, double E_s) {
  auto q_s = GSL::gslQAGIntegration<double>(
      [&](double logEp) {
        double E_p = std::pow(10., logEp);
        return std::pow(E_p, 1. - slope) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-3);
  return q_s;
}

template <typename T>
void print_secondary_source(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#E_s [GeV] - q(2.0) - q(2.4) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 40);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs, 2.0, E_s) << "\t";
    outfile << compute_source(xs, 2.4, E_s) << "\t";
    outfile << compute_source(xs, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

template <typename T>
double compute_yield(std::shared_ptr<T> xs, double slope, double E_min) {
  auto Y = GSL::gslQAGIntegration<double>(
      [&](double lny) {
        auto y = std::pow(10., lny);
        auto E_p = y * E_min;
        auto S = GSL::gslQAGIntegration<double>(
            [&](double lnEs) {
              double E_s = std::pow(10., lnEs);
              return E_s * E_s * xs->get(H1, TARGET::H, E_p, E_s);
            },
            std::log10(1e-5 * E_p), std::log10(E_p), 1000, 1e-5);
        return std::pow(y, 1. - slope) * S;
      },
      0., 4., 1000, 1e-4);

  Y /= GSL::gslQAGIntegration<double>(
      [&](double lny) {
        auto y = std::pow(10., lny);
        auto E_p = y * E_min;
        auto S = GSL::gslQAGIntegration<double>(
            [&](double lnEs) {
              double E_s = std::pow(10., lnEs);
              return E_s * xs->get(H1, TARGET::H, E_p, E_s);
            },
            std::log10(1e-5 * E_p), std::log10(E_p), 1000, 1e-5);
        return std::pow(y, 1. - slope) * S;
      },
      0., 4., 1000, 1e-4);

  return Y / E_min;
}

template <typename T>
void print_secondary_yield(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#E_s [GeV] - q(2.0) - q(2.4) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_yield(xs, 2.0, E_s) << "\t";
    outfile << compute_yield(xs, 2.4, E_s) << "\t";
    outfile << compute_yield(xs, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

template <typename T>
double compute_inelasticity(std::shared_ptr<T> xs, double T_p) {
  auto E_s = GSL::gslQAGIntegration<double>(
      [&](double y) {
        double x = std::pow(10., y);
        return x * x * xs->get(H1, TARGET::H, T_p, x * T_p);
      },
      -4., 0., 1000, 1e-4);
  E_s /= GSL::gslQAGIntegration<double>(
      [&](double y) {
        double x = std::pow(10., y);
        return x * xs->get(H1, TARGET::H, T_p, x * T_p);
      },
      -4., 0., 1000, 1e-4);
  return E_s;
}

template <typename T>
void print_secondary_inelasticity(std::shared_ptr<T> xs, std::string filename) {
  std::ofstream outfile(filename);
  outfile << "#T_proton [GeV] - Y\n";
  outfile << std::scientific;

  auto T_proton = UTILS::LogAxis(10. * cgs::GeV, 10. * cgs::TeV, 30);
  for (auto& T_p : T_proton) {
    outfile << T_p / cgs::GeV << "\t";
    outfile << compute_inelasticity(xs, T_p) << "\t";
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
  print_secondary_inelasticity<Pi0Gammas>(xs_ph, make_filename(filename, "inelasticity"));
  // print_secondary_yield<Pi0Gammas>(xs_ph, make_filename(filename, "yield"));
}

void print_ap_xsecs(SecondaryAntiprotonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs_ap = xsecs.createSecondaryAntiprotons();
  print_secondary_xsecs<SecondaryAntiprotons>(xs_ap, make_filename(filename, "xsecs"));
  print_secondary_source<SecondaryAntiprotons>(xs_ap, make_filename(filename, "source"));
  print_secondary_inelasticity<SecondaryAntiprotons>(xs_ap, make_filename(filename, "inelasticity"));
  // print_secondary_yield<SecondaryAntiprotons>(xs_ap, make_filename(filename, "yield"));
}

void print_pos_xsecs(SecondaryLeptonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryLeptons(model);
  auto xs_pos = xsecs.createSecondaryLeptons(positron);
  print_secondary_xsecs<SecondaryLeptons>(xs_pos, make_filename(filename, "xsecs"));
  print_secondary_source<SecondaryLeptons>(xs_pos, make_filename(filename, "source"));
  print_secondary_inelasticity<SecondaryLeptons>(xs_pos, make_filename(filename, "inelasticity"));
  // print_secondary_yield<SecondaryLeptons>(xs_pos, make_filename(filename, "yield"));
}

void print_terp_xsecs(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_pter = xsecs.createTertiaryProtons();
  print_secondary_xsecs<TertiaryProtons>(xs_pter, make_filename(filename, "xsecs"));
  print_secondary_source<TertiaryProtons>(xs_pter, make_filename(filename, "source"));
  print_secondary_inelasticity<TertiaryProtons>(xs_pter, make_filename(filename, "inelasticity"));
  // print_secondary_yield<TertiaryProtons>(xs_pter, make_filename(filename, "yield"));
}

int main() {
  try {
    LOG::startup_information();
    {
      neutralParticleType = NeutralParticleType::GAMMA;
      print_gamma_xsecs(Pi0GammaModels::AAFRAG, "AAFRAG_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAMAE2006, "Kamae2006_gammas");
      print_gamma_xsecs(Pi0GammaModels::KELNER2006, "Kelner2006_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_gammas");
      print_gamma_xsecs(Pi0GammaModels::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_gammas");
    }
    {
      neutralParticleType = NeutralParticleType::ALLNUS;
      print_gamma_xsecs(Pi0GammaModels::AAFRAG, "AAFRAG_nus");
      print_gamma_xsecs(Pi0GammaModels::KAMAE2006, "Kamae2006_nus");
      print_gamma_xsecs(Pi0GammaModels::KELNER2006, "Kelner2006_nus");
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