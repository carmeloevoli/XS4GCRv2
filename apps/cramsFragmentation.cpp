#include <fstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "XS4GCR.h"
#include "XS4GCR/fragmentations/Evoli2026.h"
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
  FragmentationModels model;
  std::string name;
  std::string outputFile;
  bool doGhosts;
  Evoli2026FallbackModel evoli2026Fallback;
};

ModelInfo model_info(FragmentationModels model) {
  switch (model) {
    case FragmentationModels::EVOLI2019:
      return {model, "Evoli2019", "output/crams_fragmentation_evoli2019.csv", true, Evoli2026FallbackModel::W93};
    case FragmentationModels::EVOLI2026:
      return {model, "Evoli2026_W93", "output/crams_fragmentation_evoli2026_w93.csv", true,
              Evoli2026FallbackModel::W93};
    case FragmentationModels::FLUKA4DRAGON:
      return {model, "Fluka4Dragon", "output/crams_fragmentation_fluka4dragon.csv", true, Evoli2026FallbackModel::W93};
    case FragmentationModels::WEBBER1993:
      return {model, "Webber1993", "output/crams_fragmentation_webber1993.csv", false, Evoli2026FallbackModel::W93};
    case FragmentationModels::USINEGALPROP17OPT12:
      return {model, "USINE_GALPROP17_OPT12", "output/crams_fragmentation_usine_galprop17_opt12.csv", true,
              Evoli2026FallbackModel::W93};
    case FragmentationModels::USINEGALPROP17OPT22:
      return {model, "USINE_GALPROP17_OPT22", "output/crams_fragmentation_usine_galprop17_opt22.csv", true,
              Evoli2026FallbackModel::W93};
    case FragmentationModels::USINEWEBBER03COSTE12:
      return {model, "USINE_WEBBER03_COSTE12", "output/crams_fragmentation_usine_webber03+coste12.csv", true,
              Evoli2026FallbackModel::W93};
  }
  throw std::invalid_argument("Unknown fragmentation model.");
}

ModelInfo evoli2026_model_info(Evoli2026FallbackModel fallbackModel) {
  if (fallbackModel == Evoli2026FallbackModel::ST99) {
    return {FragmentationModels::EVOLI2026, "Evoli2026_ST99", "output/crams_fragmentation_evoli2026_st99.csv", true,
            Evoli2026FallbackModel::ST99};
  }
  return {FragmentationModels::EVOLI2026, "Evoli2026_W93", "output/crams_fragmentation_evoli2026_w93.csv", true,
          Evoli2026FallbackModel::W93};
}

bool is_lighter_fragment(const Isotope& projectile, const Isotope& fragment) { return fragment.A < projectile.A; }

std::shared_ptr<Fragmentation> make_model(const ModelInfo& info) {
  if (info.model == FragmentationModels::EVOLI2026) {
    return std::make_shared<Evoli2026>(info.evoli2026Fallback);
  }

  XSECS xsecs;
  xsecs.setFragmentation(info.model);
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

void make_table(const ModelInfo& info, const std::string& isotopeFile, double TminGeV, double TmaxGeV, size_t nEnergy,
                const std::vector<Isotope>& isotopes, const std::vector<double>& energies) {
  const auto xsec = make_model(info);
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
    const std::vector<ModelInfo> models = {
        // model_info(FragmentationModels::EVOLI2019),
        evoli2026_model_info(Evoli2026FallbackModel::W93),    evoli2026_model_info(Evoli2026FallbackModel::ST99),
        model_info(FragmentationModels::FLUKA4DRAGON),        model_info(FragmentationModels::USINEGALPROP17OPT12),
        model_info(FragmentationModels::USINEGALPROP17OPT22), model_info(FragmentationModels::USINEWEBBER03COSTE12),
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
