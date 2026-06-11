#include <sys/stat.h>

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "XS4GCR.h"

using namespace XS4GCR;

namespace {

struct Isotope {
  int Z;
  int A;
  std::string name;
};

struct FragmentationChannelRow {
  Isotope projectile;
  Isotope fragment;
};

std::string model_name(FragmentationModels model) {
  if (model == FragmentationModels::FLUKA4DRAGON) return "Fluka4Dragon";
  if (model == FragmentationModels::WEBBER1993) return "Webber1993";
  throw std::invalid_argument("Unknown fragmentation model.");
}

std::string output_filename(FragmentationModels model) {
  if (model == FragmentationModels::FLUKA4DRAGON) return "output/crams_fragmentation_fluka4dragon.txt";
  if (model == FragmentationModels::WEBBER1993) return "output/crams_fragmentation_webber1993.txt";
  throw std::invalid_argument("Unknown fragmentation model.");
}

bool do_ghosts(FragmentationModels model) {
  if (model == FragmentationModels::FLUKA4DRAGON) return true;
  if (model == FragmentationModels::WEBBER1993) return false;
  throw std::invalid_argument("Unknown fragmentation model.");
}

void ensure_output_dir() {
  if (mkdir("output", 0755) != 0 && errno != EEXIST) {
    throw std::runtime_error(std::string("Cannot create output directory: ") + std::strerror(errno));
  }
}

std::vector<Isotope> read_isotopes(const std::string& filename) {
  std::ifstream input(filename.c_str());
  if (!input) throw std::runtime_error("Cannot read isotope chart: " + filename);

  std::vector<Isotope> isotopes;
  std::set<int> seen;
  std::string line;
  while (std::getline(input, line)) {
    if (line.empty() || line[0] == '#') continue;

    int Z = 0;
    int A = 0;
    std::string name;
    std::string mode;
    double halfLife = 0.0;
    std::istringstream row(line);
    if (!(row >> Z >> A >> name >> mode >> halfLife)) continue;

    const int id = 1000 * A + Z;
    if (seen.insert(id).second) isotopes.push_back(Isotope{Z, A, name});
  }

  if (isotopes.empty()) throw std::runtime_error("No isotopes found in chart: " + filename);
  return isotopes;
}

bool is_lighter_fragment(const Isotope& projectile, const Isotope& fragment) { return fragment.A < projectile.A; }

std::shared_ptr<Fragmentation> make_model(FragmentationModels model) {
  XSECS xsecs;
  xsecs.setFragmentation(model);
  return xsecs.createFragmentation();
}

std::vector<FragmentationChannelRow> collect_channels(const std::vector<Isotope>& projectiles,
                                                      const std::vector<Isotope>& fragments,
                                                      const std::shared_ptr<Fragmentation>& xsec) {
  std::vector<FragmentationChannelRow> channels;
  for (const auto& projectile : projectiles) {
    for (const auto& fragment : fragments) {
      if (!is_lighter_fragment(projectile, fragment)) continue;

      const FragmentationChannel ch = std::make_pair(PID(projectile.Z, projectile.A), PID(fragment.Z, fragment.A));
      if (xsec->hasChannel(ch)) channels.push_back(FragmentationChannelRow{projectile, fragment});
    }
  }
  return channels;
}

void write_common_header(std::ofstream& output, const std::string& modelName, const std::string& isotopeFile,
                         double TminGeV, double TmaxGeV, size_t nEnergy, size_t nChannels, bool useGhosts) {
  output << "# XS4GCR CRAMS fragmentation cross-section table\n";
  output << "# model: " << modelName << "\n";
  output << "# isotope_source: " << isotopeFile << "\n";
  output << "# channel_count: " << nChannels << "\n";
  output << "# target: H\n";
  output << "# sigma_unit: mbarn\n";
  output << "# do_ghosts: " << (useGhosts ? "true" : "false") << "\n";
  output << std::scientific << std::setprecision(8);
  output << "# energy_grid_GeV_per_n: logspace " << TminGeV << " " << TmaxGeV << " " << nEnergy << "\n";
  output << "# columns: Z_projectile A_projectile Z_fragment A_fragment sigma_H_mb(T_0) ... sigma_H_mb(T_N-1)\n";
  output << "# note: candidate projectiles and fragments are both read from isotope_source; rows require A_fragment < "
            "A_projectile and model.hasChannel(channel).\n";
}

void write_sigma_h_table(const std::string& outputFile, const std::string& modelName, const std::string& isotopeFile,
                         double TminGeV, double TmaxGeV, size_t nEnergy,
                         const std::vector<FragmentationChannelRow>& channels, const std::vector<double>& energies,
                         const std::shared_ptr<Fragmentation>& xsec, bool useGhosts) {
  std::ofstream output(outputFile.c_str());
  if (!output) throw std::runtime_error("Cannot write table: " + outputFile);

  write_common_header(output, modelName, isotopeFile, TminGeV, TmaxGeV, nEnergy, channels.size(), useGhosts);
  output << std::scientific << std::setprecision(8);

  for (const auto& row : channels) {
    const FragmentationChannel ch =
        std::make_pair(PID(row.projectile.Z, row.projectile.A), PID(row.fragment.Z, row.fragment.A));
    output << row.projectile.Z << "\t" << row.projectile.A << "\t" << row.fragment.Z << "\t" << row.fragment.A;
    for (const auto& T : energies) output << "\t" << xsec->get(ch, TARGET::H, T, useGhosts) / cgs::mbarn;
    output << "\n";
  }
}

}  // namespace

int main() {
  try {
    const auto model = FragmentationModels::FLUKA4DRAGON;
    const auto modelName = model_name(model);
    const auto useGhosts = do_ghosts(model);
    const std::string isotopeFile = "data/crchart_Z28_2020.txt";
    const std::string outputFile = output_filename(model);
    const double TminGeV = 0.01;
    const double TmaxGeV = 1e5;
    const size_t nEnergy = 7 * 32;

    if (!(TminGeV > 0.0) || !(TmaxGeV > TminGeV)) throw std::invalid_argument("Require 0 < TminGeV < TmaxGeV");
    if (nEnergy < 2) throw std::invalid_argument("nEnergy must be at least 2");
    ensure_output_dir();

    LOG::startup_information();
    const auto isotopes = read_isotopes(isotopeFile);
    const auto energies = UTILS::LogAxis(TminGeV * cgs::GeV, TmaxGeV * cgs::GeV, nEnergy);
    const auto xsec = make_model(model);
    const auto channels = collect_channels(isotopes, isotopes, xsec);

    write_sigma_h_table(outputFile, modelName, isotopeFile, TminGeV, TmaxGeV, nEnergy, channels, energies, xsec,
                        useGhosts);

    std::cout << "Wrote " << channels.size() << " fragmentation channels x " << energies.size() << " energies to "
              << outputFile << "\n";
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
