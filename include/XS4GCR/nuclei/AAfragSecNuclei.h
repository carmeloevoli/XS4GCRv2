#ifndef INCLUDE_XS4GCR_NUCLEI_AAFRAGSECNUCLEI_H
#define INCLUDE_XS4GCR_NUCLEI_AAFRAGSECNUCLEI_H

#include <memory>

#include "XS4GCR/nuclei/SecondaryNuclei.h"
#include "XS4GCR/shared/secAAfrag202.h"

namespace XS4GCR {

class AAfragSecNuclei : public SecondaryNuclei {
 public:
  explicit AAfragSecNuclei(NucleusSpecies species);

  void print() const override;

  std::shared_ptr<SecondaryNuclei> clone() override;

  double getDifferential(const PID &projectile, const TARGET &target, const double &T_proj,
                         const double &x) const override;

 protected:
  NucleusSpecies m_species;
  std::shared_ptr<AAfrag202::LookupTables> m_tables;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_NUCLEI_AAFRAGSECNUCLEI_H
