#ifndef INCLUDE_XS4GCR_COMMON_H
#define INCLUDE_XS4GCR_COMMON_H

#include "XS4GCR/pid.h"

namespace XS4GCR {

double T2beta(const double& T);

double T2gamma(const double& T);

double T2pc(const double& T, const PID& pid);

double ppInelastic(const double T);

}  // namespace XS4GCR

#endif