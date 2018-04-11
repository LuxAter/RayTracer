#ifndef RAY_LIGH_HPP_
#define RAY_LIGH_HPP_

#include <estl/vector.hpp>

#include "color.hpp"

namespace ray {
class Light {
 public:
  Light();
  Light(estl::vector::Vector<double, 3> pos, Color c, double i);

  void Illuminate(const estl::vector::Vector<double, 3>& point,
                  estl::vector::Vector<double, 3>& light_dir,
                  estl::vector::Vector<double, 3>& light_intensity,
                  double& dist);

  Color color;
  estl::vector::Vector<double, 3> position;
  double intensity;
};
}  // namespace ray

#endif  // RAY_LIGH_HPP_
