#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

auto particleType = NeutralParticleType::ALLNUS;

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

  auto xAxis = UTILS::LogAxis(1e-5, 1., 1000);
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
  auto x = UTILS::LogAxis(1e-4, 1., 4 * 64);
  std::vector<double> x2dsdx, xdsdx;
  for (auto& x_i : x) {
    x2dsdx.push_back(x_i * x_i * xs->get(H1, TARGET::H, T_p, x_i * T_p));
    xdsdx.push_back(x_i * xs->get(H1, TARGET::H, T_p, x_i * T_p));
  }
  return std::accumulate(x2dsdx.begin(), x2dsdx.end(), 0.) / std::accumulate(xdsdx.begin(), xdsdx.end(), 0.);
}

void print_inelasticity(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#T_proton [GeV] - Y\n";
  outfile << std::scientific;

  auto T_proton = UTILS::LogAxis(10. * cgs::GeV, 10. * cgs::TeV, 100);
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
  print_inelasticity(Model::AAFRAG, "AAFRAG_inelasticity");
  print_inelasticity(Model::KELNER2006, "Kelner2006_inelasticity");
  print_inelasticity(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_inelasticity");
  print_inelasticity(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_inelasticity");
  print_inelasticity(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_inelasticity");
}

double compute_source(std::shared_ptr<Pi0Gammas> xs, double slope, double E_s) {
  auto protonEnergies = UTILS::LogAxis(E_s, 1e4 * E_s, 4 * 64);
  std::vector<double> f;
  for (auto& E_p : protonEnergies) {
    auto value = std::pow(E_p, 1. - slope) * xs->get(H1, TARGET::H, E_p, E_s);
    f.push_back(value);
  }
  return std::accumulate(f.begin(), f.end(), 0.);
}

void print_source(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - q(2.2) - q(2.5) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs_gammas, 2.2, E_s) << "\t";
    outfile << compute_source(xs_gammas, 2.5, E_s) << "\t";
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

double compute_ISM_source(std::shared_ptr<Pi0Gammas> xs, const PID& projectile, double E_s) {
  auto protonEnergies = UTILS::LogAxis(E_s, 1e4 * E_s, 4 * 64);
  std::shared_ptr<Boschini2017::LIS> lis;
  if (projectile == H1)
    lis = std::make_shared<Boschini2017::H_LIS>();
  else
    lis = std::make_shared<Boschini2017::He_LIS>();
  std::vector<double> f;
  for (auto& E_p : protonEnergies) {
    auto value = E_p * lis->get(E_p);
    if (projectile == H1)
      value *= xs->get(H1, TARGET::H, E_p, E_s);
    else
      value *= xs->get(He4, TARGET::H, E_p, E_s);
    f.push_back(value);
  }
  const double n_H = 1. / cgs::cm3;
  const double dlnEp = std::log(protonEnergies[1] / protonEnergies[0]);
  return 4. * M_PI * n_H * dlnEp * std::accumulate(f.begin(), f.end(), 0.);
}

void print_ISM_source(Pi0GammaModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  auto xs_gammas = xsecs.createPi0Gammas(particleType);

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - H - He\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_ISM_source(xs_gammas, H1, E_s) << "\t";
    outfile << compute_ISM_source(xs_gammas, He4, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_ISM_source_term() {
  using Model = XS4GCR::Pi0GammaModels;
  print_ISM_source(Model::KAMAE2006, "Kamae2006_ISM_source");
  print_ISM_source(Model::AAFRAG, "AAFRAG_ISM_source");
  print_ISM_source(Model::KELNER2006, "Kelner2006_ISM_source");
  print_ISM_source(Model::KAFEXHIU2014GEANT4, "Kafexhiu2014G4_ISM_source");
  print_ISM_source(Model::KAFEXHIU2014PYTHIA8, "Kafexhiu2014P8_ISM_source");
  print_ISM_source(Model::KAFEXHIU2014SIBYLL, "Kafexhiu2014Sibyll_ISM_source");
}

int main() {
  try {
    LOG::startup_information();
    main_secondary_xsecs();
    main_inelasticity();
    main_source_term();
    main_ISM_source_term();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
