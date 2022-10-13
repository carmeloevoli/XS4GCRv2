#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

std::string make_filename(const std::string& filename) {
  auto ofile = "output/" + filename;
  ofile += "_pbar.txt";
  return ofile;
}

double compute_inelasticity(std::shared_ptr<SecondaryAntiprotons> xs, double T_p) {
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

void print_inelasticity(SecondaryAntiprotonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs_ap = xsecs.createSecondaryAntiprotons();

  std::ofstream outfile(make_filename(filename));
  outfile << "#T_proton [GeV] - Y\n";
  outfile << std::scientific;

  auto T_proton = UTILS::LogAxis(10. * cgs::GeV, 10. * cgs::TeV, 60);
  for (auto& T_p : T_proton) {
    outfile << T_p / cgs::GeV << "\t";
    outfile << compute_inelasticity(xs_ap, T_p) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_inelasticity() {
  using Model = XS4GCR::SecondaryAntiprotonModels;
  print_inelasticity(Model::AAFRAG, "AAFRAG_inelasticity");
  print_inelasticity(Model::TANNG1983, "TanNg1983_inelasticity");
  print_inelasticity(Model::DUPERRAY2003, "Duperray2003_inelasticity");
  print_inelasticity(Model::DIMAURO2014, "DiMauro2014_inelasticity");
  print_inelasticity(Model::WINKLER2017, "Winkler2017_inelasticity");
  print_inelasticity(Model::FENG2016EPOS, "Feng2016_EPOS_inelasticity");
  print_inelasticity(Model::FENG2016QGSJET, "Feng2016_QGSJET_inelasticity");
}

double compute_source(std::shared_ptr<SecondaryAntiprotons> xs, double slope, double E_s) {
  auto q_s = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        double E_p = std::exp(lnEp);
        return std::pow(E_p, 1. - slope) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log(E_s), std::log(1e4 * E_s), 1000, 1e-3);
  return q_s;
}

void print_source(SecondaryAntiprotonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs_ap = xsecs.createSecondaryAntiprotons();

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - q(2.2) - q(2.5) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 100);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs_ap, 2.2, E_s) << "\t";
    outfile << compute_source(xs_ap, 2.5, E_s) << "\t";
    outfile << compute_source(xs_ap, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_source_term() {
  using Model = XS4GCR::SecondaryAntiprotonModels;
  print_source(Model::AAFRAG, "AAFRAG_source");
  print_source(Model::TANNG1983, "TanNg1983_source");
  print_source(Model::DUPERRAY2003, "Duperray2003_source");
  print_source(Model::DIMAURO2014, "DiMauro2014_source");
  print_source(Model::WINKLER2017, "Winkler2017_source");
  print_source(Model::FENG2016EPOS, "Feng2016_EPOS_source");
  print_source(Model::FENG2016QGSJET, "Feng2016_QGSJET_source");
}

int main() {
  try {
    LOG::startup_information();
    main_secondary_xsecs();
    // main_source_term();
    // main_inelasticity();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
