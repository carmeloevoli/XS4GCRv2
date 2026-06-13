#ifndef INCLUDE_XS4GCR_PID_H
#define INCLUDE_XS4GCR_PID_H

#include <plog/Log.h>  // not used here, but many model .cpp files rely on pid.h to pull in the LOG macros

#include <cmath>
#include <ostream>
#include <stdexcept>
#include <string>

namespace XS4GCR {

class PID {
 public:
  PID() { set(0, 0); }

  // Construct a nucleus. Z and A must be physical (>= 1); anything else is a caller
  // bug and throws, rather than silently building an invalid particle.
  PID(const int& Z, const int& A) {
    if (Z < 1) throw std::invalid_argument("PID: nucleus charge Z must be >= 1, got " + std::to_string(Z));
    if (A < 1) throw std::invalid_argument("PID: nucleus mass A must be >= 1, got " + std::to_string(A));
    set(Z, A);
  }

  virtual ~PID() {}

  // Leptons are not nuclei. XS4GCR reuses PID as an identity tag for the electron and
  // positron so they can travel through the same interfaces; this named constructor
  // builds such a tag and intentionally bypasses the nucleus Z/A validation above.
  static PID makeLepton(const int& Z, const int& A) {
    PID pid;
    pid.set(Z, A);
    return pid;
  }

  int getZ() const { return m_Z; }

  int getA() const { return m_A; }

  double getZoverA() const { return (m_A > 0) ? fabs((double)m_Z / (double)m_A) : 0; }

  double getAoverZ() const { return (m_Z > 0) ? fabs((double)m_A / (double)m_Z) : 0; }

  int getId() const { return m_id; }

  bool operator==(const PID& other) const { return m_id == other.m_id; }

  bool operator!=(const PID& other) const { return m_id != other.m_id; }

  bool operator<(const PID& other) const { return m_id < other.m_id; }

  bool operator>(const PID& other) const { return m_id > other.m_id; }

  friend std::ostream& operator<<(std::ostream& stream, const PID& pid) {
    stream << "(" << pid.getA() << "," << pid.getZ() << ")";
    return stream;
  }

  std::string str() const {
    std::string ss;
    ss = "(" + std::to_string(m_Z) + "," + std::to_string(m_A) + ")";
    return ss;
  }

 protected:
  void set(const int& Z, const int& A) {
    m_Z = Z;
    m_A = A;
    m_id = A * 1000 + Z;
  }

  int m_Z;
  int m_A;
  int m_id;
};

static const PID H1 = PID(1, 1);
static const PID H2 = PID(1, 2);
static const PID He3 = PID(2, 3);
static const PID He4 = PID(2, 4);

// Lepton identity tags (see PID::makeLepton). Encoded as non-nuclei so they never
// collide with a real nucleus id, and distinguished from each other by A = +/-1.
static const PID positron = PID::makeLepton(0, 1);
static const PID electron = PID::makeLepton(0, -1);

enum TARGET { H, He };

using FragmentationChannel = std::pair<PID, PID>;

using Channel = std::pair<PID, TARGET>;

static const Channel pp = std::make_pair(H1, H);
static const Channel pHe = std::make_pair(H1, He);
static const Channel Hep = std::make_pair(He4, H);
static const Channel HeHe = std::make_pair(He4, He);

}  // namespace XS4GCR

#endif
