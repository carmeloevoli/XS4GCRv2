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

using crams::code_version;
using crams::current_date;
using crams::ensure_output_dir;
using crams::Isotope;
using crams::read_isotopes;

struct FragmentationChannelRow {
  Isotope projectile;
  Isotope fragment;
};

// Single source of truth per model: display name, output path, and whether the model
// folds in ghost (short-lived parent) contributions. Adding a model is one row here.
struct ModelInfo {
  std::string name;
  std::string outputFile;
  bool doGhosts;
};

ModelInfo model_info(FragmentationModels model) {
  switch (model) {
    case FragmentationModels::FLUKA4DRAGON:
      return {"Fluka4Dragon", "output/crams_fragmentation_fluka4dragon.csv", true};
    case FragmentationModels::WEBBER1993:
      return {"Webber1993", "output/crams_fragmentation_webber1993.csv", false};
    case FragmentationModels::USINEGALPROP17OPT12:
      return {"USINE_GALPROP17_OPT12", "output/crams_fragmentation_usine_galprop17_opt12.csv", true};
    case FragmentationModels::USINEGALPROP17OPT22:
      return {"USINE_GALPROP17_OPT22", "output/crams_fragmentation_usine_galprop17_opt22.csv", true};
    case FragmentationModels::USINEWEBBER03COSTE12:
      return {"USINE_WEBBER03_COSTE12", "output/crams_fragmentation_usine_webber03+coste12.csv", true};
  }
  throw std::invalid_argument("Unknown fragmentation model.");
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
    if (xsec->getTotal(ch, TARGET::H, T, useGhosts) > 0.0) return true;
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
    for (const auto& T : energies) output << "," << xsec->getTotal(ch, TARGET::H, T, useGhosts) / cgs::mbarn;
    output << "\n";
  }
}

void make_table(FragmentationModels model, const std::string& isotopeFile, double TminGeV, double TmaxGeV,
                size_t nEnergy, const std::vector<Isotope>& isotopes, const std::vector<double>& energies) {
  const auto info = model_info(model);
  const auto xsec = make_model(model);
  const auto channels = collect_channels(isotopes, isotopes, energies, xsec, info.doGhosts);

  write_sigma_h_table(info.outputFile, info.name, isotopeFile, TminGeV, TmaxGeV, nEnergy, channels, energies, xsec,
                      info.doGhosts);

  LOGI << "Wrote " << channels.size() << " fragmentation channels x " << energies.size() << " energies to "
       << info.outputFile;
}

}  // namespace

int main() {
  try {
    const std::string isotopeFile = "data/crams_nucleilist.csv";
    const double TminGeV = 0.01;
    const double TmaxGeV = 1e5;
    const size_t pointsPerDecade = 16;
    const size_t nEnergy = 7 * pointsPerDecade;  // 7 decades from TminGeV to TmaxGeV
    const std::vector<FragmentationModels> models = {
        FragmentationModels::FLUKA4DRAGON,
        FragmentationModels::USINEGALPROP17OPT12,
        FragmentationModels::USINEGALPROP17OPT22,
        FragmentationModels::USINEWEBBER03COSTE12,
    };

    if (!(TminGeV > 0.0) || !(TmaxGeV > TminGeV)) throw std::invalid_argument("Require 0 < TminGeV < TmaxGeV");
    if (nEnergy < 2) throw std::invalid_argument("nEnergy must be at least 2");
    ensure_output_dir();

    LOG::startup_information();
    const auto isotopes = read_isotopes(isotopeFile);
    const auto energies = UTILS::LogAxis(TminGeV * cgs::GeV, TmaxGeV * cgs::GeV, nEnergy);

    for (const auto& model : models) make_table(model, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes, energies);
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
