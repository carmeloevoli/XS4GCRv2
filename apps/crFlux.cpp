#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

int main() {
  try {
    LOG::startup_information();

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
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
