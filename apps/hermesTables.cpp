#include <fstream>
#include <numeric>

#include "XS4GCR.h"
using namespace XS4GCR;

int main() {
  try {
    LOG::startup_information();
    XSECS xsecs;
    xsecs.setPi0Gammas(Pi0GammaModels::AAFRAG);
    auto neutralParticleType = NeutralParticleType::GAMMA;
    auto xs = xsecs.createPi0Gammas(neutralParticleType);
    auto TprojAxis = UTILS::LogAxis(cgs::GeV, 1e8 * cgs::GeV, 8 * 128);
    auto xAxis = UTILS::LogAxis(1e-5, 1., 6 * 128);
    const double units = cgs::mbarn / cgs::GeV;
    std::string filename = "AAfrag2021Gamma.txt";
    std::ofstream outfile(filename);
    outfile << "#T_primary [GeV/n] - x - sigma_pp - sigma_pHe - sigma_Hep - sigma_HeHe [mbarn/GeV]\n";
    outfile << std::scientific;
    size_t counter = 0;
    for (auto& Tproj : TprojAxis) {
      std::cout << counter << " / " << TprojAxis.size() << "\n";
      counter++;
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
