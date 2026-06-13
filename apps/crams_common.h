#ifndef XS4GCR_APPS_CRAMS_COMMON_H
#define XS4GCR_APPS_CRAMS_COMMON_H

#include <sys/stat.h>

#include <cerrno>
#include <cstring>
#include <ctime>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "XS4GCR/core/git_revision.h"

// Shared helpers for the CRAMS table-writing apps (cramsInelastic, cramsFragmentation).
// Header-only: each app is its own executable, so inline definitions avoid a separate
// translation unit while staying ODR-safe.
namespace crams {

struct Isotope {
  int Z;
  int A;
};

inline std::string current_date() {
  std::time_t t = std::time(nullptr);
  char buf[11];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&t));
  return buf;
}

inline std::string code_version() {
  const std::string sha = git_sha1().substr(0, 7);
  const std::string dirty = git_has_local_changes() ? "+local" : "";
  return "XS4GCR v" + get_version() + " (git: " + sha + dirty + ")";
}

inline void ensure_output_dir() {
  if (mkdir("output", 0755) != 0 && errno != EEXIST) {
    throw std::runtime_error(std::string("Cannot create output directory: ") + std::strerror(errno));
  }
}

inline std::vector<Isotope> read_isotopes(const std::string& filename) {
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
    if (seen.insert(id).second) isotopes.push_back(Isotope{Z, A});
  }

  if (isotopes.empty()) throw std::runtime_error("No isotopes found in chart: " + filename);
  return isotopes;
}

}  // namespace crams

#endif  // XS4GCR_APPS_CRAMS_COMMON_H
