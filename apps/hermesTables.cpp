#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

int main() {
  try {
    LOG::startup_information();
    XSECS xsecs;
    xsecs.setPi0Gammas(Pi0GammaModels::AAFRAG);
    auto neutralParticleType = NeutralParticleType::ALLNUS;
    auto xs = xsecs.createPi0Gammas(neutralParticleType);
    auto xAxis = UTILS::LogAxis(1e-5, 1., 100);
    auto TprojAxis = UTILS::LogAxis(1. * cgs::GeV, 100. * cgs::PeV, 200);
    const double units = cgs::mbarn / cgs::GeV;
    std::string filename = "AAfrag2021Neutrino.txt";
    std::ofstream outfile(filename);
    outfile << "#T_primary [GeV/n] - x - sigma_pp - sigma_pHe - sigma_Hep - sigma_HeHe [mbarn/GeV]\n";
    outfile << std::scientific;
    for (auto& Tproj : TprojAxis) {
      for (auto& x : xAxis) {
        outfile << Tproj / cgs::GeV << "\t" << x << "\t";
        outfile << xs->get(H1, TARGET::H, Tproj, x * Tproj) / units << "\t";
        outfile << xs->get(H1, TARGET::He, Tproj, x * Tproj) / units << "\t";
        outfile << xs->get(He4, TARGET::H, Tproj, x * Tproj) / units << "\t";
        outfile << xs->get(He4, TARGET::He, Tproj, x * Tproj) / units << "\t";
        outfile << "\n";
      }
    }
    outfile.close();
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
