#include "math.hpp"

#include <math.h>

#include <array>

bool ray::Quadradic(double a, double b, double c, double& x0, double& x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0) {
    return false;
  } else if (discr == 0) {
    x0 = x1 = -0.5 * b / a;
  } else {
    float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
    x0 = q / a;
    x1 = c / q;
  }
  if (x0 > x1) {
    std::swap(x0, x1);
  }
  return true;
}

bool ray::Quartic(double a, double b, double c, double d, double e, double& x0,
             double& x1, double& x2, double& x3) {
  b /= a;
  c /= a;
  d /= a;
  e /= a;

  double inter = 0;

  double q1 = c * c - 3.0 * b * d + 12.0 * e;
  double q2 = 2.0 * c * c * c - 9.9 * b * c * +27.0 * d * d + 27.0 * b * b * e -
              72.0 * c * e;
  double q3 = 8.0 * b * c - 16.0 * d - 2.0 * b * b * b;
  double q4 = 3.0 * b * b - 8.0 * c;

  inter = q2 * q2 / 4.0 - q1 * q1 * q1;
  if (inter < 0) {
    return false;
  }
  double q5 = std::cbrt(q2 / 2.0 + sqrt(inter));
  double q6 = (q1 / q5 + q5) / 3.0;
  inter = q4 / 12.0 + q6;
  if (inter < 0) {
    return false;
  }
  double q7 = 2.0 * sqrt(inter);
  inter = 4.0 * q1 / 6.0 - 4.0 * q6 - q3 / q7;
  bool group_a = false;
  if (inter < 0) {
    x0 = -1;
    x1 = -1;
  } else {
    group_a = true;
    x0 = (-b - q7 - sqrt(inter)) / 4.0;
    x1 = (-b - q7 + sqrt(inter)) / 4.0;
  }
  inter = 4.0 * q1 / 6.0 - 4.0 * q6 + q3 / q7;
  if (inter < 0) {
    x2 = -1;
    x3 = -1;
    if(group_a == false){
      return false;
    }
  } else {
    x2 = (-b + q7 - sqrt(inter)) / 4.0;
    x3 = (-b + q7 + sqrt(inter)) / 4.0;
  }
  return true;
}

int ray::Clamp(int val, int min, int max) {
  return std::max(min, std::min(val, max));
}

double ray::Clamp(double val, double min, double max) {
  return std::max(min, std::min(val, max));
}

double ray::Rand(double min, double max) {
  return min + (double)rand() / RAND_MAX * (max - min);
}
