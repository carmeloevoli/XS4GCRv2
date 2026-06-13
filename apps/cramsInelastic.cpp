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

// Single source of truth per model: display name and output path. Adding a model is
// one row here.
struct ModelInfo {
  std::string name;
  std::string outputFile;
};

ModelInfo model_info(TotalInelasticModels model) {
  switch (model) {
    case TotalInelasticModels::TRIPATHI1999:
      return {"Tripathi1999", "output/crams_inelastic_tripathi99.csv"};
    case TotalInelasticModels::CROSEC:
      return {"CROSEC", "output/crams_inelastic_crosec.csv"};
    case TotalInelasticModels::LETAW1983:
      return {"Letaw1983", "output/crams_inelastic_letaw1983.csv"};
    case TotalInelasticModels::GLAUBER:
      return {"Glauber", "output/crams_inelastic_glauber.csv"};
  }
  throw std::invalid_argument("Unknown inelastic model.");
}

std::vector<Isotope> filter_supported_projectiles(const std::vector<Isotope>& isotopes) {
  std::vector<Isotope> supported;
  for (const auto& isotope : isotopes) {
    if (isotope.Z == 1 && isotope.A == 1) continue;
    supported.push_back(isotope);
  }
  return supported;
}

std::shared_ptr<TotalInelastic> make_model(TotalInelasticModels model) {
  XSECS xsecs;
  xsecs.setTotalInelastic(model);
  return xsecs.createTotalInelastic();
}

void write_common_header(std::ofstream& output, const std::string& modelName, const std::string& isotopeFile,
                         double TminGeV, double TmaxGeV, size_t nEnergy, size_t nIsotopes,
                         const std::vector<double>& energies) {
  output << "# XS4GCR CRAMS inelastic cross-section table\n";
  output << "# created: " << current_date() << "\n";
  output << "# code_version: " << code_version() << "\n";
  output << "# model: " << modelName << "\n";
  output << "# reference: TODO\n";
  output << "# isotope_source: " << isotopeFile << "\n";
  output << "# projectile_count: " << nIsotopes << "\n";
  output << "# sigma_unit: mbarn for target H\n";
  output << std::scientific << std::setprecision(8);
  output << "# energy_grid_GeV_per_n: logspace " << TminGeV << " " << TmaxGeV << " " << nEnergy << "\n";
  output << "# note: 1H is skipped because XS4GCR::TotalInelastic does not provide proton-projectile sigma.\n";
  output << "Z,A";
  for (const auto& T : energies) output << "," << T / cgs::GeV;
  output << "\n";
}

void write_sigma_h_table(const std::string& outputFile, const std::string& modelName, const std::string& isotopeFile,
                         double TminGeV, double TmaxGeV, size_t nEnergy, const std::vector<Isotope>& isotopes,
                         const std::vector<double>& energies, const std::shared_ptr<TotalInelastic>& xsec) {
  std::ofstream output(outputFile.c_str());
  if (!output) throw std::runtime_error("Cannot write table: " + outputFile);

  write_common_header(output, modelName, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes.size(), energies);
  output << std::scientific << std::setprecision(8);

  for (const auto& isotope : isotopes) {
    const auto projectile = PID(isotope.Z, isotope.A);
    output << isotope.Z << "," << isotope.A;
    for (const auto& T : energies) output << "," << xsec->getTotal(projectile, TARGET::H, T) / cgs::mbarn;
    output << "\n";
  }
}

void make_table(TotalInelasticModels model, const std::string& isotopeFile, double TminGeV, double TmaxGeV,
                size_t nEnergy, const std::vector<Isotope>& isotopes, const std::vector<double>& energies) {
  const auto info = model_info(model);
  const auto xsec = make_model(model);

  write_sigma_h_table(info.outputFile, info.name, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes, energies, xsec);

  LOGI << "Wrote " << isotopes.size() << " projectiles x " << energies.size() << " energies to " << info.outputFile;
}

}  // namespace

int main() {
  try {
    const std::string isotopeFile = "data/crams_nucleilist.csv";
    const double TminGeV = 0.01;
    const double TmaxGeV = 1e5;
    const size_t pointsPerDecade = 64;
    const size_t nEnergy = 7 * pointsPerDecade;  // 7 decades from TminGeV to TmaxGeV
    const std::vector<TotalInelasticModels> models = {
        TotalInelasticModels::GLAUBER,
        TotalInelasticModels::TRIPATHI1999,
        TotalInelasticModels::CROSEC,
    };

    if (!(TminGeV > 0.0) || !(TmaxGeV > TminGeV)) throw std::invalid_argument("Require 0 < TminGeV < TmaxGeV");
    if (nEnergy < 2) throw std::invalid_argument("nEnergy must be at least 2");
    ensure_output_dir();

    LOG::startup_information();

    const auto allIsotopes = read_isotopes(isotopeFile);
    const auto isotopes = filter_supported_projectiles(allIsotopes);
    const auto energies = UTILS::LogAxis(TminGeV * cgs::GeV, TmaxGeV * cgs::GeV, nEnergy);

    for (const auto& model : models) make_table(model, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes, energies);
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
