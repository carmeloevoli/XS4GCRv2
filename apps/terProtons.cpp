#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

std::string make_filename(const std::string& filename) {
  auto ofile = "output/" + filename;
  ofile += "_terp.txt";
  return ofile;
}

void print_secondary_xsecs(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_tp = xsecs.createTertiaryProtons();

  std::ofstream outfile(make_filename(filename));
  outfile << "#T_p [GeV] - sigma_pp - sigma_pHe - sigma_Hep - sigma_HeHe [mbarn/GeV]\n";
  outfile << std::scientific;

  const double units = cgs::mbarn;

  auto xAxis = UTILS::LogAxis(1e-5, 1., 100);
  std::vector<double> T_proj = {5. * cgs::GeV, 10. * cgs::GeV, 100. * cgs::GeV, 1. * cgs::TeV, 10. * cgs::TeV};
  for (auto& x : xAxis) {
    outfile << x << "\t";
    for (size_t i = 0; i < T_proj.size(); ++i) {
      auto T_p = T_proj[i];
      outfile << T_p * xs_tp->get(H1, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs_tp->get(H1, TARGET::He, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs_tp->get(He4, TARGET::H, T_p, x * T_p) / units << "\t";
      outfile << T_p * xs_tp->get(He4, TARGET::He, T_p, x * T_p) / units << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

void main_secondary_xsecs() {
  using Model = XS4GCR::TertiaryProtonModels;
  print_secondary_xsecs(Model::AAFRAG, "AAFRAG_xsecs");
}

double compute_inelasticity(std::shared_ptr<TertiaryProtons> xs, double T_p) {
  auto E_s = GSL::gslQAGIntegration<double>(
      [&](double y) {
        double x = std::pow(10., y);
        return x * x * xs->get(H1, TARGET::H, T_p, x * T_p);
      },
      -5., 0., 10000, 1e-4);
  E_s /= GSL::gslQAGIntegration<double>(
      [&](double y) {
        double x = std::pow(10., y);
        return x * xs->get(H1, TARGET::H, T_p, x * T_p);
      },
      -5., 0., 10000, 1e-4);
  return E_s;
}

void print_inelasticity(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_tp = xsecs.createTertiaryProtons();

  std::ofstream outfile(make_filename(filename));
  outfile << "#T_proton [GeV] - Y\n";
  outfile << std::scientific;

  auto T_proton = UTILS::LogAxis(10. * cgs::GeV, 10. * cgs::TeV, 20);
  for (auto& T_p : T_proton) {
    std::cout << T_p / cgs::GeV << "\n";
    outfile << T_p / cgs::GeV << "\t";
    outfile << compute_inelasticity(xs_tp, T_p) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_inelasticity() {
  using Model = XS4GCR::TertiaryProtonModels;
  print_inelasticity(Model::AAFRAG, "AAFRAG_inelasticity");
}

double compute_source(std::shared_ptr<TertiaryProtons> xs, double slope, double E_s) {
  auto q_s = GSL::simpsonIntegration<double>(
      [&](double lnEp) {
        double E_p = std::pow(10., lnEp);
        return std::pow(E_p, 1. - slope) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log10(E_s), std::log10(1e5 * E_s), 1000);
  return q_s;
}

void print_source(TertiaryProtonModels model, std::string filename) {
  XSECS xsecs;
  xsecs.setTertiaryProtons(model);
  auto xs_ap = xsecs.createTertiaryProtons();

  std::ofstream outfile(make_filename(filename));
  outfile << "#E_s [GeV] - q(2.0) - q(2.4) - q(2.8)\n";
  outfile << std::scientific;

  auto E_secondary = UTILS::LogAxis(10. * cgs::GeV, 100. * cgs::TeV, 10);
  for (auto& E_s : E_secondary) {
    outfile << E_s / cgs::GeV << "\t";
    outfile << compute_source(xs_ap, 2.0, E_s) << "\t";
    outfile << compute_source(xs_ap, 2.4, E_s) << "\t";
    outfile << compute_source(xs_ap, 2.8, E_s) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void main_source_term() {
  using Model = XS4GCR::TertiaryProtonModels;
  print_source(Model::AAFRAG, "AAFRAG_source");
}

int main() {
  try {
    LOG::startup_information();
    //  main_secondary_xsecs();
    //  main_inelasticity();
    main_source_term();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
