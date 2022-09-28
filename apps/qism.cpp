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

double Q_ISM_pbar(double E_s, SecondaryAntiprotonModels model, Channel channel) {
  XSECS xsecs;
  xsecs.setSecondaryAntiprotons(model);
  auto xs = xsecs.createSecondaryAntiprotons();

  const auto flux_H = Boschini2017::H_LIS();
  const auto flux_He = Boschini2017::He_LIS();
  const auto f_He = 0.1;

  double q = 0.;

  if (channel == pp) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return E_p * flux_H.get(E_p) * xs->get(H1, TARGET::H, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  if (channel == pHe) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return f_He * E_p * flux_H.get(E_p) * xs->get(H1, TARGET::He, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  if (channel == Hep) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return E_p * flux_He.get(E_p) * xs->get(He4, TARGET::H, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  if (channel == HeHe) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return f_He * E_p * flux_He.get(E_p) * xs->get(He4, TARGET::He, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  return q;
}

double Q_ISM_pos(double E_s, SecondaryLeptonModels model, Channel channel) {
  XSECS xsecs;
  xsecs.setSecondaryLeptons(model);
  auto xs = xsecs.createSecondaryLeptons(positron);

  const auto flux_H = Boschini2017::H_LIS();
  const auto flux_He = Boschini2017::He_LIS();
  const auto f_He = 0.1;

  double q = 0.;

  if (channel == pp) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return E_p * flux_H.get(E_p) * xs->get(H1, TARGET::H, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  if (channel == pHe) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return f_He * E_p * flux_H.get(E_p) * xs->get(H1, TARGET::He, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  if (channel == Hep) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return E_p * flux_He.get(E_p) * xs->get(He4, TARGET::H, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  if (channel == HeHe) {
    q = GSL::gslQAGIntegration<double>(
        [&](double lnEp) {
          auto E_p = std::pow(10., lnEp);
          return f_He * E_p * flux_He.get(E_p) * xs->get(He4, TARGET::He, E_p, E_s);
        },
        std::log10(E_s), std::log10(1e4 * E_s), 1000, 1e-4);
  }

  return q;
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
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::AAFRAG, pp) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::AAFRAG, pHe) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::AAFRAG, Hep) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::AAFRAG, HeHe) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::WINKLER2017, pp) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::WINKLER2017, pHe) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::WINKLER2017, Hep) / units << "\t";
    outfile << Q_ISM_pbar(E_s, SecondaryAntiprotonModels::WINKLER2017, HeHe) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::AAFRAG, pp) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::AAFRAG, pHe) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::AAFRAG, Hep) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::AAFRAG, HeHe) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::ORUSA2022, pp) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::ORUSA2022, pHe) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::ORUSA2022, Hep) / units << "\t";
    outfile << Q_ISM_pos(E_s, SecondaryLeptonModels::ORUSA2022, HeHe) / units << "\t";
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
