#include "XS4GCR/gsl.h"

#include <gsl/gsl_vector.h>

namespace GSL {
namespace XS4GCR {

void interpolate2d(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &z, double xi,
                   double yj) {
  //   const gsl_interp2d_type *T = gsl_interp2d_bilinear;
  //   // const size_t N = 100;           /* number of points to interpolate */
  //   // const double xa[] = {0.0, 1.0}; /* define unit square */
  //   // const double ya[] = {0.0, 1.0};

  //   const size_t nx = x.size(); /* x grid points */
  //   auto xa = gsl_vector_const_view_array(&x[0], nx);

  //   const size_t ny = y.size(); /* y grid points */
  //   auto ya = gsl_vector_const_view_array(&y[0], ny);

  //   auto za = gsl_vector_const_view_array(&z[0], nx * ny);

  //   // double za[nx * ny];
  //   gsl_spline2d *spline = gsl_spline2d_alloc(T, nx, ny);
  //   gsl_interp_accel *xacc = gsl_interp_accel_alloc();
  //   gsl_interp_accel *yacc = gsl_interp_accel_alloc();

  //   // /* set z grid values */
  //   // gsl_spline2d_set(spline, za, 0, 0, 0.0);
  //   // gsl_spline2d_set(spline, za, 0, 1, 1.0);
  //   // gsl_spline2d_set(spline, za, 1, 1, 0.5);
  //   // gsl_spline2d_set(spline, za, 1, 0, 1.0);

  //   // return gsl_stats_median(gsl_v.vector.data, 1, size);

  //   // /* initialize interpolation */
  //   gsl_spline2d_init(spline, xa, ya, za, nx, ny);

  //   // /* interpolate N values in x and y and print out grid for plotting */
  //   // for (i = 0; i < N; ++i) {
  //   //   double xi = i / (N - 1.0);

  //   //   for (j = 0; j < N; ++j) {
  //   //     double yj = j / (N - 1.0);
  //   double zij = gsl_spline2d_eval(spline, xi, yj, xacc, yacc);
  //   //   }
  //   // }

  //   gsl_spline2d_free(spline);
  //   gsl_interp_accel_free(xacc);
  //   gsl_interp_accel_free(yacc);
}

}  // namespace XS4GCR
}  // namespace GSL
