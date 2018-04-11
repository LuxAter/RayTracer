#ifndef RAY_LIGH_HPP_
#define RAY_LIGH_HPP_

#include <estl/vector.hpp>

#include "color.hpp"

namespace ray {
class Light {
 public:
  Light();
  Light(const Color& color, const double& i);
  virtual ~Light();
  virtual void Illuminate(const estl::vector::Vector<double, 3>& point,
                          estl::vector::Vector<double, 3>& dir,
                          estl::vector::Vector<double, 3>& intensity,
                          double& dist);

  Color color_;
  double intensity_;
};

class DistantLight : public Light {
 public:
  DistantLight(const estl::vector::Vector<double, 3> dir,
               const Color& color = {1.0, 1.0, 1.0}, const double& i = 1);
  void Illuminate(const estl::vector::Vector<double, 3>& point,
                  estl::vector::Vector<double, 3>& dir,
                  estl::vector::Vector<double, 3>& intensity, double& dist);

 protected:
  estl::vector::Vector<double, 3> direction_;
};

class PointLight : public Light {
 public:
  PointLight(const estl::vector::Vector<double, 3> position,
             const Color& color = {1.0, 1.0, 1.0}, const double& i = 1);
  void Illuminate(const estl::vector::Vector<double, 3>& point,
                  estl::vector::Vector<double, 3>& dir,
                  estl::vector::Vector<double, 3>& intensity, double& dist);

 protected:
  estl::vector::Vector<double, 3> position_;
};

std::unique_ptr<Light> MakeDistantLight(
    const estl::vector::Vector<double, 3> dir,
    const Color& color = {1.0, 1.0, 1.0}, const double& intensity = 1.0);
std::unique_ptr<Light> MakePointLight(
    const estl::vector::Vector<double, 3> position,
    const Color& color = {1.0, 1.0, 1.0}, const double& intensity = 1.0);

}  // namespace ray

#endif  // RAY_LIGH_HPP_
