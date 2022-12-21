#include <fstream>

#include "XS4GCR.h"
using namespace XS4GCR;

void get_inelastic_timescales(TotalInelasticModels model, std::string filename) {
  XSECS xsec;
  xsec.setTotalInelastic(model);
  const auto x_in = xsec.createTotalInelastic();

  const double f_He = 0;  // 0.08;
  const double n_gas = 1.0 / cgs::cm3;

  auto He = PID(2, 4);
  auto C = PID(6, 12);
  auto Fe = PID(26, 56);
  auto particles = std::vector<PID>{He, C, Fe};

  std::ofstream outfile(filename.c_str());
  outfile << "#T_proj [GeV/n] - tau_inelastic He, C, O, Fe [Myr]\n";
  outfile << std::scientific;

  auto T_proj = UTILS::LogAxis(0.1 * cgs::GeV, 1e6 * cgs::GeV, 10000);
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

void get_xsecs() {
  XSECS xsec;
  xsec.setTotalInelastic(TotalInelasticModels::GLAUBER);
  const auto x_in = xsec.createTotalInelastic();
  auto He = PID(2, 4);
  auto C = PID(6, 12);
  auto Fe = PID(26, 56);
  auto particles = std::vector<PID>{He, C, Fe};

  std::ofstream outfile("output/Glauber_sigmain.txt");
  outfile << "#T_proj [GeV/n] - sigma_inelastic He, C, O, Fe [mbarn]\n";
  outfile << std::scientific;

  auto T_proj = UTILS::LogAxis(cgs::GeV, 1e4 * cgs::GeV, 1000);
  for (auto T : T_proj) {
    outfile << T / cgs::GeV << "\t";
    for (auto p : particles) {
      auto sigma = x_in->get(p, TARGET::H, T);
      outfile << sigma / cgs::mbarn << "\t";
    }
    outfile << "\n";
  }
  outfile.close();
}

int main() {
  try {
    LOG::startup_information();
    {
      // get_xsecs();
      get_inelastic_timescales(TotalInelasticModels::CROSEC, "output/CROSEC_tauin.txt");
      get_inelastic_timescales(TotalInelasticModels::TRIPATHI1999, "output/Tripathi1999_tauin.txt");
      get_inelastic_timescales(TotalInelasticModels::LETAW1983, "output/Letaw1983_tauin.txt");
      get_inelastic_timescales(TotalInelasticModels::GLAUBER, "output/Glauber_tauin.txt");
    }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
  return 0;
}
