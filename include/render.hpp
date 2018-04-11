#ifndef RAY_RENDER_HPP_
#define RAY_RENDER_HPP_

#include <memory>
#include <vector>

#include <estl/vector.hpp>

#include "color.hpp"
#include "light.hpp"
#include "object.hpp"

namespace ray {
void Render(const std::vector<std::unique_ptr<Object>>& objs,
            const std::vector<std::unique_ptr<Light>>& lights, unsigned width,
            unsigned height, double fov);

Color CastRay(const estl::vector::Vector<double, 3>& start,
              const estl::vector::Vector<double, 3>& dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              const std::vector<std::unique_ptr<Light>>& lights,
              unsigned depth = 0);

bool TraceRay(const estl::vector::Vector<double, 3>& start,
              const estl::vector::Vector<double, 3>& dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              Intersect& inter);
}  // namespace ray

#endif  // RAY_RENDER_HPP_
