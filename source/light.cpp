#include "light.hpp"

#include <estl/vector.hpp>

#include "color.hpp"

using namespace estl::vector;

ray::Light::Light() {}

ray::Light::Light(Vector<double, 3> pos, Color c, double i)
    : position(pos), color(c), intensity(i) {}

void ray::Light::Illuminate(
    const estl::vector::Vector<double, 3>& point,
    estl::vector::Vector<double, 3>& light_dir,
    estl::vector::Vector<double, 3>& light_intensity, double& dist) {
  light_dir = (point - position);
  double r2 = length(light_dir);
  light_dir /= r2;
  light_intensity = Vector<double, 3>({color.r, color.g, color.b}) * (intensity / (4 * M_PI * r2));
}
