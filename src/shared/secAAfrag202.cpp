#include "XS4GCR/shared/secAAfrag202.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "XS4GCR/core/utilities.h"

namespace XS4GCR {
namespace AAfrag202 {

namespace {

const std::string kDataDir = "data/aafrag202/Tables/";
const double kSplitEnergy = 10. * cgs::GeV;  // E0 in AAfrag202.py: below this use the *L tables
const double kLogSmall = -100.;              // AAfrag202.py "small": log10 floor and fill value
const double kLogCut = kLogSmall / 3.;       // below this the interpolated spectrum is treated as zero

std::vector<std::vector<double>> readMatrix(const std::string& filename) {
  std::ifstream in(filename.c_str());
  if (!in) throw std::runtime_error("AAfrag202: cannot open " + filename);

  std::vector<std::vector<double>> rows;
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream iss(line);
    std::vector<double> row;
    double value = 0.;
    while (iss >> value) row.push_back(value);
    if (!row.empty()) rows.push_back(std::move(row));
  }
  if (rows.empty()) throw std::runtime_error("AAfrag202: empty file " + filename);
  return rows;
}

// File prefix and sub-species block index/indices per species (ALLNUS sums the 4 flavours).
struct SpeciesInfo {
  std::string prefix;
  std::vector<size_t> blocks;
};

SpeciesInfo speciesInfo(Species s) {
  switch (s) {
    case Species::GAMMA:        return {"gam", {0}};
    case Species::ELECTRON:     return {"el", {0}};
    case Species::POSITRON:     return {"el", {1}};
    case Species::ALLNUS:       return {"nu", {0, 1, 2, 3}};
    case Species::PROTON:       return {"pap", {0}};
    case Species::ANTIPROTON:   return {"pap", {1}};
    case Species::NEUTRON:      return {"nan", {0}};
    case Species::ANTINEUTRON:  return {"nan", {1}};
    case Species::ANTIDEUTERON: return {"ad", {0}};
    case Species::ANTIHELION:   return {"ah", {0}};
  }
  throw std::runtime_error("AAfrag202: unknown species");
}

std::string channelStem(Channel c) {
  switch (c) {
    case Channel::pp:   return "p_p_04";
    case Channel::pHe:  return "p_He04";
    case Channel::Hep:  return "He_p04";
    case Channel::HeHe: return "HeHe04";
    case Channel::Cp:   return "C_p_04";
    case Channel::Alp:  return "Al_p04";
    case Channel::Fep:  return "Fe_p04";
    case Channel::apP:  return "ap_p04";
    case Channel::apHe: return "apHe04";
  }
  throw std::runtime_error("AAfrag202: unknown channel");
}

// Low-energy (*L) tables only exist for the four light channels.
bool hasLowEnergyTable(Channel c) {
  return c == Channel::pp || c == Channel::pHe || c == Channel::Hep || c == Channel::HeHe;
}

// Projectile mass number: the tables index the primary by total energy = A * (energy per nucleon).
double projectileA(Channel c) {
  switch (c) {
    case Channel::pp:
    case Channel::pHe:
    case Channel::apP:
    case Channel::apHe: return 1.;
    case Channel::Hep:
    case Channel::HeHe: return 4.;
    case Channel::Cp:   return 12.;
    case Channel::Alp:  return 27.;
    case Channel::Fep:  return 56.;
  }
  return 1.;
}

}  // namespace

bool channelFor(int Zproj, int Aproj, bool targetIsHelium, Channel& out) {
  if (Zproj == 1 && Aproj == 1) {
    out = targetIsHelium ? Channel::pHe : Channel::pp;
    return true;
  }
  if (Zproj == 2 && Aproj == 4) {
    out = targetIsHelium ? Channel::HeHe : Channel::Hep;
    return true;
  }
  if (!targetIsHelium && Zproj == 6 && Aproj == 12) {
    out = Channel::Cp;
    return true;
  }
  if (!targetIsHelium && Zproj == 13 && Aproj == 27) {
    out = Channel::Alp;
    return true;
  }
  if (!targetIsHelium && Zproj == 26 && Aproj == 56) {
    out = Channel::Fep;
    return true;
  }
  return false;
}

void AAfragTable::load(const std::string& filename, size_t block) {
  const auto rows = readMatrix(filename);

  std::vector<size_t> headers;  // block boundaries: header rows have first column ~ 0
  for (size_t i = 0; i < rows.size(); ++i)
    if (rows[i][0] < 1e-50) headers.push_back(i);
  if (block >= headers.size())
    throw std::runtime_error("AAfrag202: block " + std::to_string(block) + " missing in " + filename);

  const size_t h = headers[block];
  const size_t end = (block + 1 < headers.size()) ? headers[block + 1] : rows.size();
  const auto& header = rows[h];
  if (header.size() < 3) throw std::runtime_error("AAfrag202: malformed header in " + filename);

  // Secondary energies: header columns [2, ncols)  (AAfrag202.py: E_s = data[0, 2:]).
  const size_t nSec = header.size() - 2;
  m_lgEsec.resize(nSec);
  for (size_t j = 0; j < nSec; ++j) m_lgEsec[j] = std::log10(header[2 + j] * cgs::GeV);

  // Primary energies: column 0 of the data rows of this block.
  const size_t nProj = end - (h + 1);
  if (nProj < 2) throw std::runtime_error("AAfrag202: too few primary energies in " + filename);
  m_lgEproj.resize(nProj);
  for (size_t i = 0; i < nProj; ++i) m_lgEproj[i] = std::log10(rows[h + 1 + i][0] * cgs::GeV);

  // Tabulated values: data columns [1, ncols-1)  (AAfrag202.py: value = data[..., 1:-1]).
  m_sigma = Grid<double>(nProj, nSec);
  for (size_t i = 0; i < nProj; ++i) {
    const auto& row = rows[h + 1 + i];
    for (size_t j = 0; j < nSec; ++j) {
      const double value = (1 + j < row.size()) ? row[1 + j] : 0.;
      m_sigma.get(i, j) = (value > 1e-50) ? std::log10(value) : kLogSmall;
    }
  }

  m_lgEprojRange = std::make_pair(m_lgEproj.front(), m_lgEproj.back());
  m_lgEsecRange = std::make_pair(m_lgEsec.front(), m_lgEsec.back());
}

double AAfragTable::get(double E_proj, double E_sec) const {
  if (!valid()) return 0.;
  const double lgEp = std::log10(E_proj);
  const double lgEs = std::log10(E_sec);
  if (!UTILS::inRange(lgEp, m_lgEprojRange) || !UTILS::inRange(lgEs, m_lgEsecRange)) return 0.;

  const double lgValue = GSL::interpolate2d<double>(m_lgEproj, m_lgEsec, m_sigma.get(), lgEp, lgEs);
  if (lgValue < kLogCut) return 0.;
  const double value = std::pow(10., lgValue);  // E_s * dsigma/dE_s [mb]
  return value * cgs::mbarn / E_sec;            // dsigma/dE_sec
}

LookupTables::LookupTables(Species species) : m_species(species) {
  LOGW << "loading AAfrag202 tables";
  for (Channel c : {Channel::pp, Channel::Hep, Channel::pHe, Channel::HeHe, Channel::Cp, Channel::Alp, Channel::Fep,
                    Channel::apP, Channel::apHe})
    loadChannel(c);
}

void LookupTables::loadChannel(Channel channel) {
  const auto info = speciesInfo(m_species);
  const std::string base = kDataDir + info.prefix + "_" + channelStem(channel);

  if (UTILS::fileExists(base)) {
    std::vector<AAfragTable> tables(info.blocks.size());
    for (size_t b = 0; b < info.blocks.size(); ++b) tables[b].load(base, info.blocks[b]);
    m_high[channel] = std::move(tables);
  }
  if (hasLowEnergyTable(channel) && UTILS::fileExists(base + "L")) {
    std::vector<AAfragTable> tables(info.blocks.size());
    for (size_t b = 0; b < info.blocks.size(); ++b) tables[b].load(base + "L", info.blocks[b]);
    m_low[channel] = std::move(tables);
  }
}

double LookupTables::get(Channel channel, double T_proj, double T_sec) const {
  const double E_proj = projectileA(channel) * T_proj;  // total primary energy
  const auto& tables = (E_proj < kSplitEnergy && hasLowEnergyTable(channel)) ? m_low : m_high;
  const auto it = tables.find(channel);
  if (it == tables.end()) return 0.;

  double sum = 0.;
  for (const auto& table : it->second) sum += table.get(E_proj, T_sec);
  return sum;
}

}  // namespace AAfrag202
}  // namespace XS4GCR
