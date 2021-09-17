#ifndef INCLUDE_XS4GCR_PID_H
#define INCLUDE_XS4GCR_PID_H

#include <plog/Log.h>

#include <cmath>

namespace XS4GCR {

class PID {
 public:
  PID() { set(0, 0); }

  PID(const int& Z, const int& A) {
    if (Z < 1) LOGE << "Nucleus must have charge >= 1";
    if (A < 1) LOGE << "Nucleus must have atomic number >= 1";
    set(Z, A);
  }

  virtual ~PID() {}

  void set(const int& Z, const int& A) {
    m_Z = Z;
    m_A = A;
    m_id = A * 1000 + Z;
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
  int m_Z;
  int m_A;
  int m_id;
};

enum TARGET { H, He };

using FragmentationChannel = std::pair<PID, PID>;

using Channel = std::pair<PID, TARGET>;

static const PID H1 = PID(1, 1);
static const PID H2 = PID(1, 2);
static const PID He3 = PID(2, 3);
static const PID He4 = PID(2, 4);

}  // namespace XS4GCR

#endif
