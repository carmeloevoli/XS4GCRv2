#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_FLUKA4DRAGON_H
#define INCLUDE_XS4GCR_FRAGMENTATIONS_FLUKA4DRAGON_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "XS4GCR/fragmentations/fragmentations.h"

namespace XS4GCR {

class Fluka4Dragon : public Fragmentation {
 public:
  explicit Fluka4Dragon(const std::string& dataFilename = "data/Fluka_DRAGON2_v2021.dat");

  void print() override;

  std::shared_ptr<Fragmentation> clone() override;

  bool hasChannel(const FragmentationChannel& ch) const override;

  double get(const FragmentationChannel& ch, const TARGET& target, const double& T_n, bool do_ghosts = true) override;

 protected:
  struct Table {
    std::vector<double> lgTnMeV;
    std::vector<double> sigmaH;
    std::vector<double> sigmaHe;
  };

  void readDataFile();
  static PID parsePidCode(const std::string& code);
  const std::vector<double>& selectTargetTable(const Table& table, const TARGET& target) const;

  std::string dataFilename;
  std::map<FragmentationChannel, Table> tables;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_FLUKA4DRAGON_H
