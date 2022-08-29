#ifndef XS4GCR_LIS_H
#define XS4GCR_LIS_H

#include <string>
#include <vector>

namespace XS4GCR {
namespace Boschini2017 {

class LIS {
 public:
  virtual ~LIS() = default;
  virtual void init() = 0;
  double get(const double& T_n) const;
  void read_filename();

 protected:
  std::string filename;
  std::vector<double> logR_table;
  std::vector<double> logdJdR_table;
  std::pair<int, int> id;
};

class H_LIS : public LIS {
 public:
  H_LIS() { init(); }
  void init() {
    filename = "data/proton_lis_Boschini2017.txt";
    id = std::make_pair(1, 1);
    read_filename();
  }
};

class He_LIS : public LIS {
 public:
  He_LIS() { init(); }
  void init() {
    filename = "data/helium_lis_Boschini2017.txt";
    id = std::make_pair(2, 4);
    read_filename();
  }
};

}  // namespace Boschini2017
}  // namespace XS4GCR

#endif
