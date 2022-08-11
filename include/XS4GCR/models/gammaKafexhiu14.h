#ifndef INCLUDE_XS4GCR_MODELS_GAMMAKAFEXHIU14_H
#define INCLUDE_XS4GCR_MODELS_GAMMAKAFEXHIU14_H

namespace XS4GCR {
namespace Kafexhiu14 {

enum class InteractionModel { GEANT4, PYTHIA8, QGSJET, SIBYLL };

double dsigma_dE_gamma(double T_proj, double E_gamma, InteractionModel model = InteractionModel::GEANT4);

}  // namespace Kafexhiu14
}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_MODELS_GAMMAKAFEXHIU14_H