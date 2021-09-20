#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

/**
 * @example example1.cpp
 * In this example the Carbon fragmentation timescale
 * is computed as a function of kinetic energy per nucleon
 *
 * @remark Options available for inelastic xsec : Tripathi99, CROSEC, Letaw1983
 * @remark Local ISM number density is 1 cm\f$^{-3}\f$
 * @remark Helium fraction is 0.11
 *
 * @see https://arxiv.org/abs/1711.09616
 */

void get_inelastic_timescales(TotalInelasticModels model, std::string filename) {
  XSECS xsec;
  xsec.setTotalInelastic(model);
  const auto x_in = xsec.createTotalInelastic();

  const double f_He = 0.11;
  const double n_gas = 1.0 / cgs::cm3;

  auto He = PID(2, 4);
  auto C = PID(6, 12);
  auto Fe = PID(26, 56);
  auto particles = std::vector<PID>{He, C, Fe};

  std::ofstream outfile(filename.c_str());
  outfile << "#T_proj [GeV/n] - tau_inelastic He, C, O, Fe [Myr]\n";
  outfile << std::scientific;

  auto T_proj = UTILS::LogAxis(0.1 * cgs::GeV, 1e4 * cgs::GeV, 1000);
  for (auto T : T_proj) {
    outfile << T / cgs::GeV << "\t";
    auto v = cgs::c * T2beta(T);
    for (auto p : particles) {
      auto sigma = x_in->get(p, TARGET::H, T);
      sigma += f_He * x_in->get(C, TARGET::He, T);
      auto t_f = 1. / v / n_gas / sigma;
      outfile << t_f / cgs::Myr << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

/**
 * @brief Main function for example1
 */
int main() {
  try {
    LOG::startup_information();
    {
      get_inelastic_timescales(CROSEC, "output/CROSEC_in.txt");
      get_inelastic_timescales(TRIPATHI1999, "output/Tripathi1999_in.txt");
      get_inelastic_timescales(LETAW1983, "output/Letaw1983_in.txt");
    }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
