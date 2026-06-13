#ifndef INCLUDE_XS4GCR_FRAGMENTATIONS_USINEFRAGMENTATION_H
#define INCLUDE_XS4GCR_FRAGMENTATIONS_USINEFRAGMENTATION_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "XS4GCR/fragmentations/fragmentations.h"

namespace XS4GCR {

class UsineFragmentation : public Fragmentation {
 public:
  explicit UsineFragmentation(const std::string& dataFilename = "data/usine-v3.5/sigProdGALPROP17_OPT22.dat");

  void print() const override;

  std::shared_ptr<Fragmentation> clone() override;

  bool hasChannel(const FragmentationChannel& ch) const override;

  double getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n, bool do_ghosts = true) override;

 protected:
  struct Table {
    std::vector<double> sigmaH;
    std::vector<double> sigmaHe;
  };

  void readDataFile();
  static PID parseIsotopeCode(const std::string& code);
  static int elementZ(const std::string& symbol);
  static double unitFactor(const std::string& unit);
  const std::vector<double>& selectTargetTable(const Table& table, const TARGET& target) const;

  std::string dataFilename;
  double energyMinGeV = 0.;
  double energyMaxGeV = 0.;
  size_t energyCount = 0;
  std::vector<double> lgTnGeV;
  std::map<FragmentationChannel, Table> tables;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_FRAGMENTATIONS_USINEFRAGMENTATION_H
