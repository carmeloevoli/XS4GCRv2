#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

void localFlux() {
  auto flux_H = Boschini2017::H_LIS();
  auto flux_He = Boschini2017::He_LIS();
  auto TNucleon = UTILS::LogAxis(0.1 * cgs::GeV, 1. * cgs::PeV, 1000);

  const double units = 1. / (cgs::GeV * cgs::m2 * cgs::second);

  std::ofstream outfile("output/lis.txt");
  outfile << "#T_n [GeV] - H - He\n";
  outfile << std::scientific;
  for (auto T_n : TNucleon) {
    outfile << T_n / cgs::GeV << "\t";
    outfile << flux_H.get(T_n) / units << "\t";
    outfile << flux_He.get(T_n) / units << "\t";
    outfile << "\n";
  }
}

std::pair<double, double> Q_ISM_pbar(double E_s) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(SecondaryAntiprotonModels::AAFRAG);
  auto xs = xsecs.createSecondaryAntiprotons();

  const auto flux_H = Boschini2017::H_LIS();
  const auto flux_He = Boschini2017::He_LIS();

  auto q_H = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        double E_p = std::exp(lnEp);
        return E_p * flux_H.get(E_p) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log(E_s), std::log(1e4 * E_s), 1000, 1e-3);
  q_H *= 4. * M_PI;

  auto q_He = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        double E_p = std::exp(lnEp);
        return E_p * flux_He.get(E_p) * xs->get(He4, TARGET::H, E_p, E_s);
      },
      std::log(E_s), std::log(1e4 * E_s), 1000, 1e-3);
  q_He *= 4. * M_PI;

  return {q_H, q_He};
}

void Q_ISM() {
  std::ofstream outfile("output/qism.txt");
  outfile << "#E_s [GeV] - H->pbar - He->pbar - H->pos - He->pos - H->gamma - He->gamma\n";
  outfile << std::scientific;
  auto secEnergy = UTILS::LogAxis(cgs::GeV, cgs::TeV, 10);
  const double units = 1;
  for (auto E_s : secEnergy) {
    outfile << E_s / cgs::GeV << "\t";
    auto q_pbar = Q_ISM_pbar(E_s);
    outfile << q_pbar.first / units << "\t";
    outfile << q_pbar.second / units << "\t";
    // outfile << flux_H.get(T_n) / units << "\t";
    // outfile << flux_He.get(T_n) / units << "\t";
    outfile << "\n";
  }
}

int main() {
  try {
    LOG::startup_information();
    localFlux();
    Q_ISM();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
