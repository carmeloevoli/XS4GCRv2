#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

auto particleType = NeutralParticleType::GAMMA;

std::string make_filename(const std::string& filename) {
  auto ofile = "output/" + filename;
  if (particleType == NeutralParticleType::GAMMA)
    ofile += "_gammas.txt";
  else
    ofile += "_nus.txt";
  return ofile;
}

void print_secondary_xsecs(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#T_gamma [GeV] - sigma_pp - sigma_pHe - sigma_Hep - sigma_HeHe [mbarn/GeV]\n";
  outfile << std::scientific;

  const double units = cgs::mbarn;

  auto xAxis = UTILS::LogAxis(1e-5, 1., 100);
  std::vector<double> T_proj = {5. * cgs::GeV, 10. * cgs::GeV, 100. * cgs::GeV, 1. * cgs::TeV, 10. * cgs::TeV};
  for (auto& x : xAxis) {
    outfile << x << "\t";
    for (size_t i = 0; i < T_proj.size(); ++i) {
      auto T_p = T_proj[i];
      outfile << T_p * xs_gammas->get(H1, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs_gammas->get(H1, TARGET::He, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs_gammas->get(He4, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs_gammas->get(He4, TARGET::He, T_p, x * T_p) / units << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

void main_secondary_xsecs() {
  using Model = XS4GCR::Pi0GammaModels;
  print_secondary_xsecs(Model::KAMAE2006, "Kamae2006_xsecs");
  print_secondary_xsecs(Model::AAFRAG, "AAFRAG_xsecs");
  print_secondary_xsecs(Model::KELNER2006, "Kelner2006_xsecs");
  print_secondary_xsecs(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_xsecs");
  print_secondary_xsecs(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_xsecs");
  print_secondary_xsecs(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_xsecs");
}

double compute_inelasticity(std::shared_ptr<Pi0Gammas> xs, double T_p) {
  auto E_s = GSL::gslQAGIntegration<double>(
      [&](double y) {
        double x = std::exp(y);
        return x * x * xs->get(H1, TARGET::H, T_p, x * T_p);
      },
      -12., 0., 1000, 1e-3);
  E_s /= GSL::gslQAGIntegration<double>(
      [&](double y) {
        double x = std::exp(y);
        return x * xs->get(H1, TARGET::H, T_p, x * T_p);
      },
      -12., 0., 1000, 1e-3);
  return E_s;
}

void print_inelasticity(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#T_proton [GeV] - Y\n";
  outfile << std::scientific;

  auto T_proton = UTILS::LogAxis(10. * cgs::GeV, 10. * cgs::TeV, 60);
  for (auto& T_p : T_proton) {
    outfile << T_p / cgs::GeV << "\t";
    outfile << compute_inelasticity(xs_gammas, T_p) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_inelasticity() {
  using Model = XS4GCR::Pi0GammaModels;
  print_inelasticity(Model::KAMAE2006, "Kamae2006_inelasticity");
  print_inelasticity(Model::KELNER2006, "Kelner2006_inelasticity");
  print_inelasticity(Model::AAFRAG, "AAFRAG_inelasticity");
  print_inelasticity(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_inelasticity");
  print_inelasticity(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_inelasticity");
  print_inelasticity(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_inelasticity");
}

double compute_source(std::shared_ptr<Pi0Gammas> xs, double slope, double E_s) {
  auto q_s = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        double E_p = std::exp(lnEp);
        return std::pow(E_p, 1. - slope) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log(E_s), std::log(1e4 * E_s), 1000, 1e-3);
  return q_s;
}

void print_source(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - q(2.2) - q(2.4) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs_gammas, 2.0, E_s) << "\t";
    outfile << compute_source(xs_gammas, 2.4, E_s) << "\t";
    outfile << compute_source(xs_gammas, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_source_term() {
  using Model = XS4GCR::Pi0GammaModels;
  print_source(Model::KAMAE2006, "Kamae2006_source");
  print_source(Model::AAFRAG, "AAFRAG_source");
  print_source(Model::KELNER2006, "Kelner2006_source");
  print_source(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_source");
  print_source(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_source");
  print_source(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_source");
}

double compute_yield(std::shared_ptr<Pi0Gammas> xs, double slope, double E_min) {
  auto Y = GSL::gslQAGIntegration<double>(
      [&](double lny) {
        auto y = std::exp(lny);
        auto E_p = y * E_min;
        auto S = GSL::gslQAGIntegration<double>(
            [&](double lnEs) {
              double E_s = std::exp(lnEs);
              return E_s * E_s * xs->get(H1, TARGET::H, E_p, E_s);
            },
            std::log(1e-5 * E_p), std::log(E_p), 1000, 1e-5);
        return std::pow(y, 1. - slope) * S;
      },
      std::log(1.), std::log(1e5), 1000, 1e-4);

  Y /= GSL::gslQAGIntegration<double>(
      [&](double lny) {
        auto y = std::exp(lny);
        auto E_p = y * E_min;
        auto S = GSL::gslQAGIntegration<double>(
            [&](double lnEs) {
              double E_s = std::exp(lnEs);
              return E_s * xs->get(H1, TARGET::H, E_p, E_s);
            },
            std::log(1e-5 * E_p), std::log(E_p), 1000, 1e-5);
        return std::pow(y, 1. - slope) * S;
      },
      std::log(1.), std::log(1e5), 1000, 1e-4);

  return Y / E_min;
}

void print_yield(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - Y(2.2) - Y(2.4) - Y(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_yield(xs_gammas, 2.0, E_s) << "\t";
    outfile << compute_yield(xs_gammas, 2.4, E_s) << "\t";
    outfile << compute_yield(xs_gammas, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_yield() {
  using Model = XS4GCR::Pi0GammaModels;
  print_yield(Model::KAMAE2006, "Kamae2006_yield");
  print_yield(Model::AAFRAG, "AAFRAG_yield");
  print_yield(Model::KELNER2006, "Kelner2006_yield");
  print_yield(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_yield");
  print_yield(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_yield");
  print_yield(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_yield");
}

int main() {
  try {
    LOG::startup_information();
    // main_secondary_xsecs();
    // main_source_term();
    main_inelasticity();
    main_yield();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
