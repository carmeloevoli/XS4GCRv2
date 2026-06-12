#include <sys/stat.h>

#include <cerrno>
#include <cstring>
#include <ctime>
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
#include "XS4GCR/core/git_revision.h"

using namespace XS4GCR;

namespace {

struct Isotope {
  int Z;
  int A;
  std::string name;
};

struct ModelOutput {
  TotalInelasticModels model;
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

std::string model_name(TotalInelasticModels model) {
  if (model == TotalInelasticModels::TRIPATHI1999) return "Tripathi1999";
  if (model == TotalInelasticModels::CROSEC) return "CROSEC";
  if (model == TotalInelasticModels::LETAW1983) return "Letaw1983";
  if (model == TotalInelasticModels::GLAUBER) return "Glauber";
  throw std::invalid_argument("Unknown inelastic model.");
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
    for (const auto& T : energies) output << "," << xsec->get(projectile, TARGET::H, T) / cgs::mbarn;
    output << "\n";
  }
}

void make_table(const ModelOutput& config, const std::string& isotopeFile, double TminGeV, double TmaxGeV,
                size_t nEnergy, const std::vector<Isotope>& isotopes, const std::vector<double>& energies) {
  const auto model = config.model;
  const auto modelName = model_name(model);
  const auto xsec = make_model(model);

  write_sigma_h_table(config.outputFile, modelName, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes, energies, xsec);

  LOGI << "Wrote " << isotopes.size() << " projectiles x " << energies.size() << " energies to " << config.outputFile;
}

}  // namespace

int main() {
  try {
    const std::string isotopeFile = "data/crams_nucleilist.csv";
    const double TminGeV = 0.01;
    const double TmaxGeV = 1e5;
    const size_t nEnergy = 7 * 64;
    const std::vector<ModelOutput> tables = {
        {TotalInelasticModels::GLAUBER, "output/crams_inelastic_glauber.csv"},
        {TotalInelasticModels::TRIPATHI1999, "output/crams_inelastic_tripathi99.csv"},
        {TotalInelasticModels::CROSEC, "output/crams_inelastic_crosec.csv"},
    };

    if (!(TminGeV > 0.0) || !(TmaxGeV > TminGeV)) throw std::invalid_argument("Require 0 < TminGeV < TmaxGeV");
    if (nEnergy < 2) throw std::invalid_argument("nEnergy must be at least 2");
    ensure_output_dir();

    LOG::startup_information();

    const auto allIsotopes = read_isotopes(isotopeFile);
    const auto isotopes = filter_supported_projectiles(allIsotopes);
    const auto energies = UTILS::LogAxis(TminGeV * cgs::GeV, TmaxGeV * cgs::GeV, nEnergy);

    for (const auto& table : tables) make_table(table, isotopeFile, TminGeV, TmaxGeV, nEnergy, isotopes, energies);
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
    return 1;
  }
  return 0;
}
