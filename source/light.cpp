#include "light.hpp"

#include "basic/vector.hpp"

#include "color.hpp"
#include "math.hpp"

using namespace estl::base;

ray::Light::Light() {}
ray::Light::Light(const Color& color, const double& i)
    : color_(color), intensity_(i), type_(NONE) {}
ray::Light::Light(const Color& color, const double& i, const LightType& type)
    : color_(color), intensity_(i), type_(type) {}
ray::Light::~Light() {}

void ray::Light::Illuminate(const estl::base::Vec3d& point,
                            estl::base::Vec3d& dir,
                            estl::base::Vec3d& intensity, double& dist) {}
void ray::Light::Move(const estl::base::Vec3d& pos) {}
void ray::Light::Rotate(const estl::base::Vec3d& dir) {}

ray::DistantLight::DistantLight(const estl::base::Vec3d dir, const Color& color,
                                const double& i)
    : Light(color, i, DISTANT) {
  direction_ = Normalize(dir);
}
void ray::DistantLight::Illuminate(const estl::base::Vec3d& point,
                                   estl::base::Vec3d& dir,
                                   estl::base::Vec3d& intensity, double& dist) {
  dir = direction_;
  intensity = Vec3d(color_.r, color_.g, color_.b) * intensity_;
  dist = INFINITY;
}
void ray::DistantLight::Rotate(const estl::base::Vec3d& dir) {
  direction_ = dir;
}

ray::PointLight::PointLight(const estl::base::Vec3d position,
                            const Color& color, const double& i)
    : Light(color, i, POINT) {
  position_ = position;
}
void ray::PointLight::Illuminate(const estl::base::Vec3d& point,
                                 estl::base::Vec3d& dir,
                                 estl::base::Vec3d& intensity, double& dist) {
  dir = point - position_;
  dist = Length(dir);
  dir /= dist;
  if (intensity_ != -1) {
    intensity = Vec3d(color_.r, color_.g, color_.b) *
                Clamp(intensity_ / (4 * M_PI * pow(dist, 2)), 0.0, 1.0);
  } else {
    intensity = Vec3d(color_.r, color_.g, color_.b);
  }
}
void ray::PointLight::Move(const estl::base::Vec3d& pos) {
  position_ = pos;
}

ray::AreaLight::AreaLight(const estl::base::Vec3d& position,
                          const estl::base::Vec3d& dir, const double& width,
                          const double& height, const unsigned& samples,
                          const Color& color, const double& i)
    : Light(color, i, AREA) {
  position_ = position;
  direction_ = dir;
  height_ = height;
  width_ = width;
  samples_ = samples;
}

void ray::AreaLight::Illuminate(const estl::base::Vec3d& point,
                                estl::base::Vec3d& dir,
                                estl::base::Vec3d& intensity, double& dist) {
  dir = point - position_;
  dist = Length(dir);
  dir /= dist;
  if (intensity_ != -1) {
    intensity = Vec3d(color_.r, color_.g, color_.b) * intensity_ /
                (4 * M_PI * pow(dist, 2));
  } else {
    intensity = Vec3d(color_.r, color_.g, color_.b);
  }
}
void ray::AreaLight::Move(const estl::base::Vec3d& pos) {
  position_ = pos;
}
void ray::AreaLight::Rotate(const estl::base::Vec3d& dir) {
  direction_ = dir;
}

std::unique_ptr<ray::Light> ray::MakeDistantLight(const estl::base::Vec3d dir,
                                                  const Color& color,
                                                  const double& intensity) {
  return std::unique_ptr<Light>(new DistantLight(dir, color, intensity));
}
std::unique_ptr<ray::Light> ray::MakePointLight(
    const estl::base::Vec3d position, const Color& color,
    const double& intensity) {
  return std::unique_ptr<Light>(new PointLight(position, color, intensity));
}
std::unique_ptr<ray::Light> ray::MakeAreaLight(
    const estl::base::Vec3d position, const estl::base::Vec3d& direction,
    const double& width, const double& height, const unsigned& samples,
    const Color& color, const double& intensity) {
  return std::unique_ptr<Light>(new AreaLight(
      position, direction, width, height, samples, color, intensity));
}
