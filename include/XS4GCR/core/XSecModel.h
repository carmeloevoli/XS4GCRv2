#ifndef INCLUDE_XS4GCR_CORE_XSECMODEL_H
#define INCLUDE_XS4GCR_CORE_XSECMODEL_H

#include <memory>

namespace XS4GCR {

// Interface shared by every cross-section model family (inelastic, fragmentation,
// secondary antiprotons/leptons/neutrals/nuclei). It captures the two
// operations every family needs identically; the cross-section accessor get(...)
// stays on each family base because its signature differs (total vs. differential).
//
// Derived is the family base class itself (CRTP), so clone() remains covariant:
// each family hands back a shared_ptr to its own base type rather than to XSecModel.
template <typename Derived>
class XSecModel {
 public:
  virtual ~XSecModel() = default;

  // Log a one-line description of the model and its reference.
  virtual void print() const = 0;

  // Deep copy, used by the XSECS factory to hand out independent instances.
  virtual std::shared_ptr<Derived> clone() = 0;
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_CORE_XSECMODEL_H
