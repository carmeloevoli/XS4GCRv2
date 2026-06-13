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

// Neutral secondary species this tool tabulates for HERMES.
enum class Secondary { GAMMA, NEUTRINO };

std::string secondary_label(Secondary s) { return (s == Secondary::GAMMA) ? "gamma" : "neutrino"; }

NeutralParticleType neutral_type(Secondary s) {
  return (s == Secondary::GAMMA) ? NeutralParticleType::GAMMA : NeutralParticleType::ALLNUS;
}

// Single source of truth per model: enum, display name, file stem, and which secondaries
// it can produce. Adding a model is one row here. (Kafexhiu2014 is gamma-only.)
struct ModelInfo {
  Pi0GammaModels model;
  std::string name;
  std::string stem;
  bool gammas;
  bool neutrinos;
};

const std::vector<ModelInfo>& model_catalog() {
  static const std::vector<ModelInfo> catalog = {
      {Pi0GammaModels::KAMAE2006, "Kamae2006", "kamae2006", true, true},
      {Pi0GammaModels::KELNER2006, "Kelner2006", "kelner2006", true, true},
      {Pi0GammaModels::KAFEXHIU2014GEANT4, "Kafexhiu2014_Geant4", "kafexhiu2014_geant4", true, false},
      {Pi0GammaModels::KAFEXHIU2014PYTHIA8, "Kafexhiu2014_Pythia8", "kafexhiu2014_pythia8", true, false},
      {Pi0GammaModels::KAFEXHIU2014SIBYLL, "Kafexhiu2014_Sibyll", "kafexhiu2014_sibyll", true, false},
      {Pi0GammaModels::AAFRAG, "AAfrag2021", "aafrag2021", true, true},
  };
  return catalog;
}

bool supports(const ModelInfo& info, Secondary s) { return (s == Secondary::GAMMA) ? info.gammas : info.neutrinos; }

std::shared_ptr<Pi0Gammas> make_model(Pi0GammaModels model, Secondary s) {
  XSECS xsecs;
  xsecs.setPi0Gammas(model);
  return xsecs.createPi0Gammas(neutral_type(s));
}

void write_common_header(std::ofstream& output, const std::string& modelName, Secondary s, double TpMinGeV,
                         double TpMaxGeV, size_t nT, double xMin, double xMax, size_t nX) {
  output << "# XS4GCR HERMES " << secondary_label(s) << " production cross-section table\n";
  output << "# created: " << current_date() << "\n";
  output << "# code_version: " << code_version() << "\n";
  output << "# model: " << modelName << "\n";
  output << "# reference: TODO\n";
  output << "# secondary: " << secondary_label(s) << (s == Secondary::NEUTRINO ? " (sum over all flavours)" : "") << "\n";
  output << "# channels: pp, pHe, Hep, HeHe (projectile on H/He target; zero where a model lacks the channel)\n";
  output << "# quantity: T_primary * d(sigma)/d(E_secondary)\n";
  output << "# sigma_unit: mbarn\n";
  output << std::scientific << std::setprecision(8);
  output << "# T_primary_grid_GeV_per_n: logspace " << TpMinGeV << " " << TpMaxGeV << " " << nT << "\n";
  output << "# x_grid: logspace " << xMin << " " << xMax << " " << nX << "  (x = E_secondary / T_primary)\n";
  output << "# columns: T_primary[GeV/n]  x  sigma_pp  sigma_pHe  sigma_Hep  sigma_HeHe  [mbarn]\n";
}

void write_table(const ModelInfo& info, Secondary s, double TpMinGeV, double TpMaxGeV, size_t nT, double xMin,
                 double xMax, size_t nX) {
  const std::string outputFile = "output/hermes_" + secondary_label(s) + "_" + info.stem + ".txt";
  std::ofstream output(outputFile.c_str());
  if (!output) throw std::runtime_error("Cannot write table: " + outputFile);

  const auto xsec = make_model(info.model, s);
  const auto Tprimary = UTILS::LogAxis(TpMinGeV * cgs::GeV, TpMaxGeV * cgs::GeV, nT);
  const auto xGrid = UTILS::LogAxis(xMin, xMax, nX);

  write_common_header(output, info.name, s, TpMinGeV, TpMaxGeV, nT, xMin, xMax, nX);
  output << std::scientific << std::setprecision(8);

  for (const auto& Tp : Tprimary) {
    for (const auto& x : xGrid) {
      output << Tp / cgs::GeV << "\t" << x << "\t" << xsec->getDifferential(H1, TARGET::H, Tp, x) / cgs::mbarn << "\t"
             << xsec->getDifferential(H1, TARGET::He, Tp, x) / cgs::mbarn << "\t"
             << xsec->getDifferential(He4, TARGET::H, Tp, x) / cgs::mbarn << "\t"
             << xsec->getDifferential(He4, TARGET::He, Tp, x) / cgs::mbarn << "\n";
    }
  }

  LOGI << "Wrote " << nT << " x " << nX << " (T_primary, x) points to " << outputFile;
}

}  // namespace

int main() {
  try {
    const double TpMinGeV = 1.0;
    const double TpMaxGeV = 1e8;
    const double xMin = 1e-5;
    const double xMax = 1.0;
    const size_t pointsPerDecade = 64;
    const size_t nT = 8 * pointsPerDecade;  // 8 decades in T_primary
    const size_t nX = 5 * pointsPerDecade;  // 5 decades in x

    if (!(TpMinGeV > 0.0) || !(TpMaxGeV > TpMinGeV)) throw std::invalid_argument("Require 0 < TpMinGeV < TpMaxGeV");
    if (!(xMin > 0.0) || !(xMax > xMin)) throw std::invalid_argument("Require 0 < xMin < xMax");
    if (nT < 2 || nX < 2) throw std::invalid_argument("grid sizes must be at least 2");
    ensure_output_dir();

    LOG::startup_information();

    for (Secondary s : {Secondary::GAMMA, Secondary::NEUTRINO}) {
      for (const auto& info : model_catalog()) {
        if (!supports(info, s)) continue;
        write_table(info, s, TpMinGeV, TpMaxGeV, nT, xMin, xMax, nX);
      }
    }
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
