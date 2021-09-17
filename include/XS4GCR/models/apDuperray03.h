#ifndef INCLUDE_XS4GCR_MODELS_APDUPERRAY03_H
#define INCLUDE_XS4GCR_MODELS_APDUPERRAY03_H

namespace XS4GCR {
namespace apDuperray03 {

enum Channel { PP, PHE, HEP };

double dsigma_dT(double T_p, double T_ap, Channel channel);

}  // namespace apDuperray03
}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_MODELS_APDUPERRAY03_H