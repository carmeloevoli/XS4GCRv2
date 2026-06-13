#ifndef INCLUDE_XS4GCR_SHARED_SECAAFRAG202_H
#define INCLUDE_XS4GCR_SHARED_SECAAFRAG202_H

#include <plog/Log.h>

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/grid.h"
#include "XS4GCR/core/gsl.h"

namespace XS4GCR {
namespace AAfrag202 {

// AAfrag v2.02 (Kachelriess, Ostapchenko & Tjemsland 2023; arXiv:2206.00998).
// The data files are self-describing: a file may stack several sub-species "blocks",
// each a matrix whose header row holds the secondary energies E_s [GeV] and whose
// remaining rows start with the primary energy E_p [GeV] followed by the tabulated
// value E_s * dsigma/dE_s [mb]. Blocks are delimited by header rows (first column ~0).
//
// One AAfragTable holds a single block and reproduces AAfrag202.py: it interpolates
// log10(value) bilinearly in (log10 E_p, log10 E_s), with 0 outside the tabulated range.

class AAfragTable {
 public:
  AAfragTable() = default;

  // Load sub-species `block` (0-based) of `filename`. Pairs each tabulated value with
  // the secondary energy exactly as AAfrag202.py does (E_s = header[2:], value = row[1:-1]).
  void load(const std::string& filename, size_t block);

  bool valid() const { return m_sigma.size() > 0; }

  // dsigma/dE_sec for a primary of total energy E_proj on this block's target, returning a
  // secondary of energy E_sec (both in cgs energy units). Zero outside the tabulated range.
  double get(double E_proj, double E_sec) const;

 private:
  std::vector<double> m_lgEproj;  // log10(E_p) over the cgs energy grid
  std::vector<double> m_lgEsec;   // log10(E_s) over the cgs energy grid
  std::pair<double, double> m_lgEprojRange;
  std::pair<double, double> m_lgEsecRange;
  Grid<double> m_sigma;  // log10(value), value = E_s * dsigma/dE_s [mb]
};

// Secondary species. Each maps to a file prefix and a sub-species block index; ALLNUS is
// the sum over the four neutrino flavour blocks.
enum class Species {
  GAMMA,
  ELECTRON,
  POSITRON,
  ALLNUS,
  PROTON,
  ANTIPROTON,
  NEUTRON,
  ANTINEUTRON,
  ANTIDEUTERON,
  ANTIHELION
};

// Reaction channels (projectile on target). C/Al/Fe are heavier projectiles on H; apP/apHe
// (antiproton projectile) exist only for antideuteron/antihelium.
enum class Channel { pp, Hep, pHe, HeHe, Cp, Alp, Fep, apP, apHe };

// Map a cosmic-ray projectile (Z, A) on an ISM target (H or He) to a reaction channel.
// Returns false for combinations AAfrag does not tabulate (e.g. C/Al/Fe on He).
bool channelFor(int Zproj, int Aproj, bool targetIsHelium, Channel& out);

class LookupTables {
 public:
  explicit LookupTables(Species species);
  virtual ~LookupTables() = default;

  // dsigma/dE_sec for a projectile of kinetic energy per nucleon T_proj producing a
  // secondary of energy T_sec (cgs). Returns 0 for channels/energies the model omits.
  double get(Channel channel, double T_proj, double T_sec) const;

 private:
  void loadChannel(Channel channel);

  Species m_species;
  // Per channel: low-energy (E_proj < 10 GeV) and high-energy blocks. ALLNUS keeps the
  // four flavour blocks to be summed; every other species keeps exactly one.
  std::map<Channel, std::vector<AAfragTable>> m_low;
  std::map<Channel, std::vector<AAfragTable>> m_high;
};

}  // namespace AAfrag202
}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_SHARED_SECAAFRAG202_H
