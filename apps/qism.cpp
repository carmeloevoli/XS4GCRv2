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

std::pair<double, double> Q_ISM_pbar(double E_s, SecondaryAntiprotonModels model) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs = xsecs.createSecondaryAntiprotons();

  const auto flux_H = Boschini2017::H_LIS();
  const auto flux_He = Boschini2017::He_LIS();

  auto q_H = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        auto E_p = std::pow(10., lnEp);
        return E_p * flux_H.get(E_p) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);

  auto q_He = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        auto E_p = std::pow(10., lnEp);
        return E_p * flux_He.get(E_p) * xs->get(He4, TARGET::H, E_p, E_s);
      },
      std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);

  return {q_H, q_He};
}

std::pair<double, double> Q_ISM_pos(double E_s, SecondaryLeptonModels model) {
  XSECS xsecs;
  xsecs.setSecondaryLeptons(model);
  auto xs = xsecs.createSecondaryLeptons(positron);

  const auto flux_H = Boschini2017::H_LIS();
  const auto flux_He = Boschini2017::He_LIS();

  auto q_H = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        auto E_p = std::pow(10., lnEp);
        return E_p * flux_H.get(E_p) * xs->get(H1, TARGET::H, E_p, E_s);
      },
      std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);

  auto q_He = GSL::gslQAGIntegration<double>(
      [&](double lnEp) {
        auto E_p = std::pow(10., lnEp);
        return E_p * flux_He.get(E_p) * xs->get(He4, TARGET::H, E_p, E_s);
      },
      std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);

  return {q_H, q_He};
}

double grammage(double E) {
  auto value = 10. * cgs::gram / cgs::cm2;
  value *= std::pow(E / 10. / cgs::GeV, -0.5);
  return value / cgs::protonMass;
}

void Q_ISM() {
  std::ofstream outfile("output/qism.txt");
  outfile << "#E_s [GeV] - H->pbar - He->pbar - H->pos - He->pos - H->gamma - He->gamma\n";
  outfile << std::scientific;
  auto secEnergy = UTILS::LogAxis(cgs::GeV, cgs::TeV, 30);
  const double units = 1. / cgs::GeV / cgs::second / cgs::steradian;
  for (auto E_s : secEnergy) {
    outfile << E_s / cgs::GeV << "\t";
    std::cout << E_s / cgs::GeV << "\n";
    outfile << grammage(E_s) / (1. / cgs::m2) << "\t";
    {
      auto q_pbar = Q_ISM_pbar(E_s, SecondaryAntiprotonModels::DIMAURO2014);
      outfile << q_pbar.first / units << "\t";
      outfile << q_pbar.second / units << "\t";
    }
    {
      auto q_pbar = Q_ISM_pbar(E_s, SecondaryAntiprotonModels::WINKLER2017);
      outfile << q_pbar.first / units << "\t";
      outfile << q_pbar.second / units << "\t";
    }
    {
      auto q_pos = Q_ISM_pos(E_s, SecondaryLeptonModels::KAMAE2006);
      outfile << q_pos.first / units << "\t";
      outfile << q_pos.second / units << "\t";
    }
    {
      auto q_pos = Q_ISM_pos(E_s, SecondaryLeptonModels::ORUSA2022);
      outfile << q_pos.first / units << "\t";
      outfile << q_pos.second / units << "\t";
    }
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
