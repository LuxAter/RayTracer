#ifndef RAY_LIGH_HPP_
#define RAY_LIGH_HPP_

#include "basic/vector.hpp"

#include <memory>

#include "color.hpp"

namespace ray {
  enum LightType{
    NONE,
    DISTANT,
    POINT,
    AREA
  };

class Light {
 public:
  Light();
  Light(const Color& color, const double& i);
  Light(const Color& color, const double& i, const LightType& type);
  virtual ~Light();
  virtual void Illuminate(const estl::base::Vec3d& point,
                          estl::base::Vec3d& dir, estl::base::Vec3d& intensity,
                          double& dist);
  Color color_;
  double intensity_;
  LightType type_;
};

class DistantLight : public Light {
 public:
  DistantLight(const estl::base::Vec3d dir,
               const Color& color = {1.0, 1.0, 1.0}, const double& i = 1);
  void Illuminate(const estl::base::Vec3d& point, estl::base::Vec3d& dir,
                  estl::base::Vec3d& intensity, double& dist);

 protected:
  estl::base::Vec3d direction_;
};

class PointLight : public Light {
 public:
  PointLight(const estl::base::Vec3d position,
             const Color& color = {1.0, 1.0, 1.0}, const double& i = 1);
  void Illuminate(const estl::base::Vec3d& point, estl::base::Vec3d& dir,
                  estl::base::Vec3d& intensity, double& dist);

 protected:
  estl::base::Vec3d position_;
};

class AreaLight : public Light {
 public:
  AreaLight(const estl::base::Vec3d& position, const estl::base::Vec3d& dir,
            const double& width, const double& height, const unsigned& samples,
            const Color& color = {1.0, 1.0, 1.0}, const double& i = 1);
  void Illuminate(const estl::base::Vec3d& point, estl::base::Vec3d& dir,
                  estl::base::Vec3d& intensity, double& dist);

  estl::base::Vec3d position_, direction_;
  double width_, height_;
  unsigned samples_;
};

std::unique_ptr<Light> MakeDistantLight(const estl::base::Vec3d dir,
                                        const Color& color = {1.0, 1.0, 1.0},
                                        const double& intensity = 1.0);
std::unique_ptr<Light> MakePointLight(const estl::base::Vec3d position,
                                      const Color& color = {1.0, 1.0, 1.0},
                                      const double& intensity = -1.0);
std::unique_ptr<Light> MakeAreaLight(const estl::base::Vec3d position,
                                     const estl::base::Vec3d& direction,
                                     const double& width, const double& height,
                                     const unsigned& samples,
                                     const Color& color = {1.0, 1.0, 1.0},
                                     const double& intensity = -1.0);

}  // namespace ray

#endif  // RAY_LIGH_HPP_
