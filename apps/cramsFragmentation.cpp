#include <sys/stat.h>

#include <cerrno>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "XS4GCR.h"
#include "XS4GCR/core/git_revision.h"

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

struct ModelOutput {
  FragmentationModels model;
  std::string outputFile;
};

std::string current_date() {
  std::time_t t = std::time(nullptr);
  char buf[11];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&t));
  return buf;
}

std::string code_version() {
  const std::string sha = git_sha1().substr(0, 7);
  const std::string dirty = git_has_local_changes() ? "+local" : "";
  return "XS4GCR v" + get_version() + " (git: " + sha + dirty + ")";
}

std::string model_name(FragmentationModels model) {
  if (model == FragmentationModels::FLUKA4DRAGON) return "Fluka4Dragon";
  if (model == FragmentationModels::WEBBER1993) return "Webber1993";
  if (model == FragmentationModels::USINEGALPROP17OPT12) return "USINE_GALPROP17_OPT12";
  if (model == FragmentationModels::USINEGALPROP17OPT22) return "USINE_GALPROP17_OPT22";
  if (model == FragmentationModels::USINEWEBBER03COSTE12) return "USINE_WEBBER03_COSTE12";
  throw std::invalid_argument("Unknown fragmentation model.");
}

std::string output_filename(FragmentationModels model) {
  if (model == FragmentationModels::FLUKA4DRAGON) return "output/crams_fragmentation_fluka4dragon.csv";
  if (model == FragmentationModels::WEBBER1993) return "output/crams_fragmentation_webber1993.csv";
  if (model == FragmentationModels::USINEGALPROP17OPT22) return "output/crams_fragmentation_usine_galprop17_opt22.csv";
  if (model == FragmentationModels::USINEGALPROP17OPT12) return "output/crams_fragmentation_usine_galprop17_opt12.csv";
  if (model == FragmentationModels::USINEWEBBER03COSTE12)
    return "output/crams_fragmentation_usine_webber03+coste12.csv";
  throw std::invalid_argument("Unknown fragmentation model.");
}

bool do_ghosts(FragmentationModels model) {
  if (model == FragmentationModels::FLUKA4DRAGON) return true;
  if (model == FragmentationModels::WEBBER1993) return false;
  if (model == FragmentationModels::USINEGALPROP17OPT12) return true;
  if (model == FragmentationModels::USINEGALPROP17OPT22) return true;
  if (model == FragmentationModels::USINEWEBBER03COSTE12) return true;
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
    char sep = 0;
    std::istringstream row(line);
    if (!(row >> Z >> sep >> A)) continue;

    const int id = 1000 * A + Z;
    const std::string name = std::to_string(A) + std::to_string(Z);
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

bool has_nonzero_sigma(const FragmentationChannel& ch, const std::vector<double>& energies,
                       const std::shared_ptr<Fragmentation>& xsec, bool useGhosts) {
  for (const auto& T : energies) {
    if (xsec->get(ch, TARGET::H, T, useGhosts) > 0.0) return true;
  }
  return false;
}

std::vector<FragmentationChannelRow> collect_channels(const std::vector<Isotope>& projectiles,
                                                      const std::vector<Isotope>& fragments,
                                                      const std::vector<double>& energies,
                                                      const std::shared_ptr<Fragmentation>& xsec, bool useGhosts) {
  std::vector<FragmentationChannelRow> channels;
  for (const auto& projectile : projectiles) {
    for (const auto& fragment : fragments) {
      if (!is_lighter_fragment(projectile, fragment)) continue;

      const FragmentationChannel ch = std::make_pair(PID(projectile.Z, projectile.A), PID(fragment.Z, fragment.A));
      if (xsec->hasChannel(ch) && has_nonzero_sigma(ch, energies, xsec, useGhosts))
        channels.push_back(FragmentationChannelRow{projectile, fragment});
    }
  }
  return channels;
}

void write_common_header(std::ofstream& output, const std::string& modelName, const std::string& isotopeFile,
                         double TminGeV, double TmaxGeV, size_t nEnergy, size_t nChannels, bool useGhosts,
                         const std::vector<double>& energies) {
  output << "# XS4GCR CRAMS fragmentation cross-section table\n";
  output << "# created: " << current_date() << "\n";
  output << "# code_version: " << code_version() << "\n";
  output << "# model: " << modelName << "\n";
  output << "# reference: TODO\n";
  output << "# isotope_source: " << isotopeFile << "\n";
  output << "# channel_count: " << nChannels << "\n";
  output << "# target: H\n";
  output << "# sigma_unit: mbarn\n";
  output << "# do_ghosts: " << (useGhosts ? "true" : "false") << "\n";
  output << std::scientific << std::setprecision(8);
  output << "# energy_grid_GeV_per_n: logspace " << TminGeV << " " << TmaxGeV << " " << nEnergy << "\n";
  output << "# note: candidate projectiles and fragments are both read from isotope_source; rows require A_fragment < "
            "A_projectile, model.hasChannel(channel), and nonzero H-target sigma on the output energy grid.\n";
  output << "Z_proj,A_proj,Z_frag,A_frag";
  for (const auto& T : energies) output << "," << T / cgs::GeV;
  output << "\n";
}

void write_sigma_h_table(const std::string& outputFile, const std::string& modelName, const std::string& isotopeFile,
                         double TminGeV, double TmaxGeV, size_t nEnergy,
                         const std::vector<FragmentationChannelRow>& channels, const std::vector<double>& energies,
                         const std::shared_ptr<Fragmentation>& xsec, bool useGhosts) {
  std::ofstream output(outputFile.c_str());
  if (!output) throw std::runtime_error("Cannot write table: " + outputFile);

  write_common_header(output, modelName, isotopeFile, TminGeV, TmaxGeV, nEnergy, channels.size(), useGhosts, energies);
  output << std::scientific << std::setprecision(8);

  for (const auto& row : channels) {
    const FragmentationChannel ch =
        std::make_pair(PID(row.projectile.Z, row.projectile.A), PID(row.fragment.Z, row.fragment.A));
    output << row.projectile.Z << "," << row.projectile.A << "," << row.fragment.Z << "," << row.fragment.A;
    for (const auto& T : energies) output << "," << xsec->get(ch, TARGET::H, T, useGhosts) / cgs::mbarn;
    output << "\n";
  }
}

void make_table(const ModelOutput& config, const std::string& isotopeFile, double TminGeV, double TmaxGeV,
                size_t nEnergy, const std::vector<Isotope>& isotopes, const std::vector<double>& energies) {
  const auto model = config.model;
  const auto modelName = model_name(model);
  const auto useGhosts = do_ghosts(model);
  const auto xsec = make_model(model);
  const auto channels = collect_channels(isotopes, isotopes, energies, xsec, useGhosts);

  write_sigma_h_table(config.outputFile, modelName, isotopeFile, TminGeV, TmaxGeV, nEnergy, channels, energies, xsec,
                      useGhosts);

  LOGI << "Wrote " << channels.size() << " fragmentation channels x " << energies.size() << " energies to "
       << config.outputFile;
}

}  // namespace

int main() {
  try {
    const std::string isotopeFile = "data/crams_nucleilist.csv";
    const double TminGeV = 0.01;
    const double TmaxGeV = 1e5;
    const size_t nEnergy = 7 * 16;
    const std::vector<ModelOutput> tables = {
        {FragmentationModels::FLUKA4DRAGON, output_filename(FragmentationModels::FLUKA4DRAGON)},
        {FragmentationModels::USINEGALPROP17OPT12, output_filename(FragmentationModels::USINEGALPROP17OPT12)},
        {FragmentationModels::USINEGALPROP17OPT22, output_filename(FragmentationModels::USINEGALPROP17OPT22)},
        {FragmentationModels::USINEWEBBER03COSTE12, output_filename(FragmentationModels::USINEWEBBER03COSTE12)},
    };

    if (!(TminGeV > 0.0) || !(TmaxGeV > TminGeV)) throw std::invalid_argument("Require 0 < TminGeV < TmaxGeV");
    if (nEnergy < 2) throw std::invalid_argument("nEnergy must be at least 2");
    ensure_output_dir();

    LOG::startup_information();
    const auto isotopes = read_isotopes(isotopeFile);
    const auto energies = UTILS::LogAxis(TminGeV * cgs::GeV, TmaxGeV * cgs::GeV, nEnergy);

    for (const auto& table : tables) make_table(table, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes, energies);
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
