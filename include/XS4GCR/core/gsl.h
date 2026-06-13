#ifndef XS4GCR_GSL_H
#define XS4GCR_GSL_H

#include <gsl/gsl_integration.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_vector.h>

#include <algorithm>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace XS4GCR {
namespace GSL {

namespace detail {

struct IntegrationWorkspaceDeleter {
  void operator()(gsl_integration_workspace *workspace) const { gsl_integration_workspace_free(workspace); }
};

struct RootSolverDeleter {
  void operator()(gsl_root_fsolver *solver) const { gsl_root_fsolver_free(solver); }
};

struct Spline2dDeleter {
  void operator()(gsl_spline2d *spline) const { gsl_spline2d_free(spline); }
};

struct InterpAccelDeleter {
  void operator()(gsl_interp_accel *accel) const { gsl_interp_accel_free(accel); }
};

using IntegrationWorkspacePtr = std::unique_ptr<gsl_integration_workspace, IntegrationWorkspaceDeleter>;
using RootSolverPtr = std::unique_ptr<gsl_root_fsolver, RootSolverDeleter>;
using Spline2dPtr = std::unique_ptr<gsl_spline2d, Spline2dDeleter>;
using InterpAccelPtr = std::unique_ptr<gsl_interp_accel, InterpAccelDeleter>;

inline IntegrationWorkspacePtr makeIntegrationWorkspace(size_t limit) {
  IntegrationWorkspacePtr workspace(gsl_integration_workspace_alloc(limit));
  if (!workspace) throw std::bad_alloc();
  return workspace;
}

inline RootSolverPtr makeRootSolver(const gsl_root_fsolver_type *solverType) {
  RootSolverPtr solver(gsl_root_fsolver_alloc(solverType));
  if (!solver) throw std::bad_alloc();
  return solver;
}

inline Spline2dPtr makeSpline2d(const gsl_interp2d_type *type, size_t nx, size_t ny) {
  Spline2dPtr spline(gsl_spline2d_alloc(type, nx, ny));
  if (!spline) throw std::bad_alloc();
  return spline;
}

inline InterpAccelPtr makeInterpAccel() {
  InterpAccelPtr accel(gsl_interp_accel_alloc());
  if (!accel) throw std::bad_alloc();
  return accel;
}

template <typename T, typename Function>
double invokeGslFunction(double x, void *params) {
  auto *function = static_cast<Function *>(params);
  return static_cast<double>((*function)(static_cast<T>(x)));
}

template <typename T, typename Function>
gsl_function makeGslFunction(Function &function) {
  gsl_function gslFunction;
  gslFunction.function = &invokeGslFunction<T, Function>;
  gslFunction.params = &function;
  return gslFunction;
}

}  // namespace detail

template <typename T, typename Function>
T gslQAGSIntegration(Function &&callable, T start, T stop, size_t limit, double rel_error = 1e-4) {
  using FunctionType = typename std::decay<Function>::type;
  FunctionType function(std::forward<Function>(callable));

  double a = static_cast<double>(start);
  double b = static_cast<double>(stop);
  double abs_error = 0.0;  // disabled
  double result;
  double error;

  gsl_function gslFunction = detail::makeGslFunction<T>(function);
  auto workspace = detail::makeIntegrationWorkspace(limit);

  gsl_integration_qags(&gslFunction, a, b, abs_error, rel_error, limit, workspace.get(), &result, &error);

  return T(result);
}

template <typename T, typename Function>
T gslQAGIntegration(Function &&callable, T start, T stop, size_t limit, double rel_error = 1e-4) {
  using FunctionType = typename std::decay<Function>::type;
  FunctionType function(std::forward<Function>(callable));

  double a = static_cast<double>(start);
  double b = static_cast<double>(stop);
  double abs_error = 0.0;  // disabled
  int key = GSL_INTEG_GAUSS41;
  double result;
  double error;

  gsl_function gslFunction = detail::makeGslFunction<T>(function);
  auto workspace = detail::makeIntegrationWorkspace(limit);

  gsl_integration_qag(&gslFunction, a, b, abs_error, rel_error, limit, key, workspace.get(), &result, &error);

  return T(result);
}

template <typename T, typename Function>
T gslQAGIUIntegration(Function &&callable, T start, size_t limit, double rel_error = 1e-4) {
  using FunctionType = typename std::decay<Function>::type;
  FunctionType function(std::forward<Function>(callable));

  double a = static_cast<double>(start);
  double abs_error = 0.0;  // disabled
  double result;
  double error;

  gsl_function gslFunction = detail::makeGslFunction<T>(function);
  auto workspace = detail::makeIntegrationWorkspace(limit);

  gsl_integration_qagiu(&gslFunction, a, abs_error, rel_error, limit, workspace.get(), &result, &error);

  return T(result);
}

template <typename T, typename Function>
T simpsonIntegration(Function &&callable, T start, T stop, int N = 100) {
  if (N <= 0 || N % 2 != 0) {
    throw std::invalid_argument("simpsonIntegration requires a positive, even number of intervals");
  }

  using FunctionType = typename std::decay<Function>::type;
  FunctionType function(std::forward<Function>(callable));

  const T a = start;
  const T b = stop;

  const T h = (b - a) / N;
  const T XI0 = function(a) + function(b);

  T XI1 = 0, XI2 = 0;

  for (int i = 1; i < N; ++i) {
    const T X = a + i * h;
    if (i % 2 == 0)
      XI2 = XI2 + function(X);
    else
      XI1 = XI1 + function(X);
  }

  return h * (XI0 + 2 * XI2 + 4 * XI1) / 3.0;
}

template <typename T>
T interpolate2d(const std::vector<T> &x, const std::vector<T> &y, const std::vector<T> &z, T xi, T yj) {
  if (x.empty() || y.empty() || z.size() != x.size() * y.size()) {
    throw std::invalid_argument("interpolate2d requires x.size() * y.size() grid values");
  }

  const gsl_interp2d_type *I = gsl_interp2d_bilinear;
  const size_t nx = x.size();
  const size_t ny = y.size();

  std::vector<double> xa(x.begin(), x.end());
  std::vector<double> ya(y.begin(), y.end());
  std::vector<double> za(nx * ny);

  auto spline = detail::makeSpline2d(I, nx, ny);
  auto xacc = detail::makeInterpAccel();
  auto yacc = detail::makeInterpAccel();

  /* set z grid values */
  for (size_t i = 0; i < nx; i++)
    for (size_t j = 0; j < ny; j++) {
      gsl_spline2d_set(spline.get(), za.data(), i, j, static_cast<double>(z[j + ny * i]));
    }

  /* initialize interpolation */
  gsl_spline2d_init(spline.get(), xa.data(), ya.data(), za.data(), nx, ny);

  T zij = static_cast<T>(
      gsl_spline2d_eval(spline.get(), static_cast<double>(xi), static_cast<double>(yj), xacc.get(), yacc.get()));

  return zij;
}

template <typename T>
T linearInterpolate(const std::vector<T> &X, const std::vector<T> &Y, T x) {
  if (X.size() != Y.size()) {
    throw std::invalid_argument("linearInterpolate requires X and Y to have the same size");
  }

  auto it = std::upper_bound(X.begin(), X.end(), x);
  if (it == X.begin()) return 0;
  if (it == X.end()) return 0;

  const size_t i = it - X.begin() - 1;
  return Y[i] + (x - X[i]) * (Y[i + 1] - Y[i]) / (X[i + 1] - X[i]);
}

template <typename T, typename Function>
T rootFinder(Function &&callable, T xLower, T xUpper, int maxIter, double relError = 1e-4) {
  if (maxIter <= 0) {
    throw std::invalid_argument("rootFinder requires maxIter to be positive");
  }

  using FunctionType = typename std::decay<Function>::type;
  FunctionType function(std::forward<Function>(callable));

  int status;
  int iter = 0;
  const gsl_root_fsolver_type *solverType = gsl_root_fsolver_brent;

  T r = 0;

  gsl_function gslFunction = detail::makeGslFunction<T>(function);
  auto solver = detail::makeRootSolver(solverType);

  gsl_root_fsolver_set(solver.get(), &gslFunction, static_cast<double>(xLower), static_cast<double>(xUpper));

  do {
    iter++;
    status = gsl_root_fsolver_iterate(solver.get());
    r = static_cast<T>(gsl_root_fsolver_root(solver.get()));
    xLower = static_cast<T>(gsl_root_fsolver_x_lower(solver.get()));
    xUpper = static_cast<T>(gsl_root_fsolver_x_upper(solver.get()));
    status = gsl_root_test_interval(static_cast<double>(xLower), static_cast<double>(xUpper), 0, relError);
  } while (status == GSL_CONTINUE && iter < maxIter);

  return r;
}

}  // namespace GSL
}  // namespace XS4GCR

#endif
