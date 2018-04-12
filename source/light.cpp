#include "light.hpp"

#include <estl/vector.hpp>

#include "color.hpp"

using namespace estl::vector;

ray::Light::Light() {}
ray::Light::Light(const Color& color, const double& i)
    : color_(color), intensity_(i) {}
ray::Light::~Light() {}

void ray::Light::Illuminate(const estl::vector::Vector<double, 3>& point,
                            estl::vector::Vector<double, 3>& dir,
                            estl::vector::Vector<double, 3>& intensity,
                            double& dist) {}

ray::DistantLight::DistantLight(const estl::vector::Vector<double, 3> dir,
                                const Color& color, const double& i)
    : Light(color, i) {
  direction_ = normalize(dir);
}
void ray::DistantLight::Illuminate(const estl::vector::Vector<double, 3>& point,
                                   estl::vector::Vector<double, 3>& dir,
                                   estl::vector::Vector<double, 3>& intensity,
                                   double& dist) {
  dir = direction_;
  intensity = Vector<double, 3>({color_.r, color_.g, color_.b}) * intensity_;
  dist = INFINITY;
}

ray::PointLight::PointLight(const estl::vector::Vector<double, 3> position,
                            const Color& color, const double& i)
    : Light(color, i) {
  position_ = position;
}
void ray::PointLight::Illuminate(const estl::vector::Vector<double, 3>& point,
                                 estl::vector::Vector<double, 3>& dir,
                                 estl::vector::Vector<double, 3>& intensity,
                                 double& dist) {
  dir = point - position_;
  dist = length(dir);
  dir /= dist;
  if (intensity_ != -1) {
    intensity = Vector<double, 3>({color_.r, color_.g, color_.b}) * intensity_ /
                (4 * M_PI * pow(dist, 2));
  }else{
    intensity = Vector<double, 3>({color_.r, color_.g, color_.b});
  }
}

std::unique_ptr<ray::Light> ray::MakeDistantLight(
    const estl::vector::Vector<double, 3> dir, const Color& color,
    const double& intensity) {
  return std::unique_ptr<Light>(new DistantLight(dir, color, intensity));
}
std::unique_ptr<ray::Light> ray::MakePointLight(
    const estl::vector::Vector<double, 3> position, const Color& color,
    const double& intensity) {
  return std::unique_ptr<Light>(new PointLight(position, color, intensity));
}
