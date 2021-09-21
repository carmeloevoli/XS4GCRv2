#ifndef INCLUDE_XS4GCR_MODELS_SECKAMAE06_H
#define INCLUDE_XS4GCR_MODELS_SECKAMAE06_H

namespace XS4GCR {
namespace Kamae06 {

enum cparamParticles { KELECTRON, KPOSITRON, KGAMMA, KNU };
double getCparamSigma(cparamParticles particle, double T_proton, double T_secondary);

}  // namespace Kamae06
}  // namespace XS4GCR

#endif