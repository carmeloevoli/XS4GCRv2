#include <fstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "XS4GCR.h"
#include "crams_common.h"

using namespace XS4GCR;

namespace {

using crams::ensure_output_dir;
using crams::Isotope;
using crams::read_isotopes;

std::shared_ptr<Fragmentation> make_model() {
  XSECS xsecs;
  xsecs.setFragmentation(FragmentationModels::EVOLI2026);
  return xsecs.createFragmentation();
}

void write_table(const std::shared_ptr<Fragmentation>& sigma, const std::vector<Isotope>& isotopes,
                 const std::string& filename, bool doGhosts) {
  std::ofstream output(filename.c_str());
  if (!output) throw std::runtime_error("Cannot write fragmentation table: " + filename);

  const double Tmin = 10. * cgs::MeV;
  const double Tmax = 1. * cgs::TeV;
  const size_t energyCount = 5 * 32;
  const auto energies = UTILS::LogAxis(Tmin, Tmax, energyCount);

  output << std::setprecision(3);
  output << "0 0 0 0 ";
  for (size_t i = 0; i < energies.size(); ++i) {
    output << std::scientific << energies.at(i) / cgs::GeV;
    if (i + 1 < energies.size()) output << " ";
  }
  output << "\n";

  size_t counter = 0;
  for (const auto& projectile : isotopes) {
    for (const auto& fragment : isotopes) {
      if (projectile.A <= fragment.A) continue;

      ++counter;
      const FragmentationChannel ch = std::make_pair(PID(projectile.Z, projectile.A), PID(fragment.Z, fragment.A));
      output << fragment.Z << " " << fragment.A << " ";
      output << projectile.Z << " " << projectile.A << " ";
      for (size_t i = 0; i < energies.size(); ++i) {
        output << sigma->getTotal(ch, TARGET::H, energies.at(i), doGhosts) / cgs::mbarn;
        if (i + 1 < energies.size()) output << " ";
      }
      output << "\n";
    }
  }

  LOGI << "computed " << counter << " fragmentation reactions in " << filename;
}

}  // namespace

int main() {
  try {
    const std::string isotopeFile = "data/crams_nucleilist.csv";
    ensure_output_dir();

    LOG::startup_information();
    const auto sigma = make_model();
    const auto isotopes = read_isotopes(isotopeFile);

    write_table(sigma, isotopes, "output/crxsecs_fragmentation_Evoli2026_direct.txt", false);
    write_table(sigma, isotopes, "output/crxsecs_fragmentation_Evoli2026_cumulative.txt", true);
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
