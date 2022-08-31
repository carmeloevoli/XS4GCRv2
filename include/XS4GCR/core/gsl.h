#ifndef XS4GCR_GSL_H
#define XS4GCR_GSL_H

#include <gsl/gsl_integration.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_vector.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace XS4GCR {
namespace GSL {

template <typename T>
T gslQAGSIntegration(std::function<T(T)> f, T start, T stop, size_t LIMIT, double rel_error = 1e-4) {
  double a = static_cast<double>(start);
  double b = static_cast<double>(stop);
  double abs_error = 0.0;  // disabled
  double result;
  double error;

  gsl_function F = {.function = [](double x, void *vf) -> double {
                      auto &func = *static_cast<std::function<double(double)> *>(vf);
                      return func(x);
                    },
                    .params = &f};

  gsl_integration_workspace *workspace_ptr = gsl_integration_workspace_alloc(LIMIT);
  gsl_integration_qags(&F, a, b, abs_error, rel_error, LIMIT, workspace_ptr, &result, &error);
  gsl_integration_workspace_free(workspace_ptr);

  return T(result);
}

template <typename T>
T gslQAGIntegration(std::function<T(T)> f, T start, T stop, int LIMIT, double rel_error = 1e-4) {
  double a = static_cast<double>(start);
  double b = static_cast<double>(stop);
  double abs_error = 0.0;  // disabled
  int key = GSL_INTEG_GAUSS31;
  double result;
  double error;

  gsl_function F = {.function = [](double x, void *vf) -> double {
                      auto &func = *static_cast<std::function<double(double)> *>(vf);
                      return func(x);
                    },
                    .params = &f};

  gsl_integration_workspace *workspace_ptr = gsl_integration_workspace_alloc(LIMIT);
  gsl_integration_qag(&F, a, b, abs_error, rel_error, LIMIT, key, workspace_ptr, &result, &error);
  gsl_integration_workspace_free(workspace_ptr);

  return T(result);
}

template <typename T>
T gslQAGIUIntegration(std::function<T(T)> f, T start, int LIMIT, double rel_error = 1e-4) {
  double a = static_cast<double>(start);
  double abs_error = 0.0;  // disabled
  double result;
  double error;

  gsl_function F = {.function = [](double x, void *vf) -> double {
                      auto &func = *static_cast<std::function<double(double)> *>(vf);
                      return func(x);
                    },
                    .params = &f};

  gsl_integration_workspace *workspace_ptr = gsl_integration_workspace_alloc(LIMIT);
  gsl_integration_qagiu(&F, a, abs_error, rel_error, LIMIT, workspace_ptr, &result, &error);
  gsl_integration_workspace_free(workspace_ptr);

  return T(result);
}

template <typename T>
T interpolate2d(const std::vector<T> &x, const std::vector<T> &y, const std::vector<T> &z, T xi, T yj) {
  const gsl_interp2d_type *I = gsl_interp2d_bicubic;
  const T *xa = &x[0];
  const T *ya = &y[0];
  const size_t nx = x.size();
  const size_t ny = y.size();
  T za[nx * ny];
  gsl_spline2d *spline = gsl_spline2d_alloc(I, nx, ny);
  gsl_interp_accel *xacc = gsl_interp_accel_alloc();
  gsl_interp_accel *yacc = gsl_interp_accel_alloc();

  /* set z grid values */
  for (size_t i = 0; i < nx; i++)
    for (size_t j = 0; j < ny; j++) {
      gsl_spline2d_set(spline, za, i, j, z.at(j + ny * i));
    }

  /* initialize interpolation */
  gsl_spline2d_init(spline, xa, ya, za, nx, ny);

  T zij = gsl_spline2d_eval(spline, xi, yj, xacc, yacc);

  gsl_spline2d_free(spline);
  gsl_interp_accel_free(xacc);
  gsl_interp_accel_free(yacc);
  return zij;
}

template <typename T>
T linearInterpolate(const std::vector<T> &X, const std::vector<T> &Y, T x) {
  auto it = std::upper_bound(X.begin(), X.end(), x);
  if (it == X.begin()) return 0;
  if (it == X.end()) return 0;

  const size_t i = it - X.begin() - 1;
  return Y[i] + (x - X[i]) * (Y[i + 1] - Y[i]) / (X[i + 1] - X[i]);
}

}  // namespace GSL
}  // namespace XS4GCR

#endif