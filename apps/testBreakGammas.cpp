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

double compute_source(std::shared_ptr<Pi0Gammas> xs, double E_s) {
  auto q_s = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        auto E_p = std::exp(lnEp);
        auto phi_p = (E_p > 1. * cgs::TeV) ? std::pow(E_p, -2.8) : std::pow(E_p, -2.0);
        return E_p * phi_p * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log(E_s), std::log(1e4 * E_s), 1000, 1e-3);
  return q_s;
}

void print_source(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - q\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs_gammas, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_source_term() {
  using Model = XS4GCR::Pi0GammaModels;
  print_source(Model::KAMAE2006, "Kamae2006_break");
  print_source(Model::KELNER2006, "Kelner2006_break");
  print_source(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_break");
  print_source(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_break");
  print_source(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_break");
  print_source(Model::AAFRAG, "AAFRAG_break");
}

int main() {
  try {
    LOG::startup_information();
    main_source_term();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
