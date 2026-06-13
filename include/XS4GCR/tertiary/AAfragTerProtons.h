#ifndef INCLUDE_XS4GCR_TERTIARY_AAFRAG_H
#define INCLUDE_XS4GCR_TERTIARY_AAFRAG_H

#include "XS4GCR/nuclei/AAfragSecNuclei.h"
#include "XS4GCR/tertiary/TertiaryProtons.h"

namespace XS4GCR {

class AAfragTerProtons : public AAfragSecNuclei {
 public:
  AAfragTerProtons() : AAfragSecNuclei(NucleusSpecies::PROTON) {}

  std::shared_ptr<TertiaryProtons> clone() override;
};

inline std::shared_ptr<TertiaryProtons> AAfragTerProtons::clone() { return std::make_shared<AAfragTerProtons>(*this); }

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_TERTIARY_AAFRAG_H
