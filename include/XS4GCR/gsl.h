#ifndef XS4GCR_GSL_H
#define XS4GCR_GSL_H

#include <gsl/gsl_integration.h>

#include <functional>

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

}  // namespace GSL
}  // namespace XS4GCR

#endif