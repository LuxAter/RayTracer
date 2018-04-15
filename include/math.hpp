#ifndef RAY_MAP_HPP_
#define RAY_MAP_HPP_

#include <math.h>
#include <array>

#ifndef INFINITY
#define INFINITY 1e20
#endif

namespace ray {
bool Quadradic(double a, double b, double c, double& x0, double& x1);
double Clamp(double val, double min, double max);
double Rand(double min=0.0, double max=1.0);
}  // namespace ray

#endif  // RAY_MAP_HPP_
