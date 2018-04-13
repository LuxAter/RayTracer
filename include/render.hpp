#ifndef RAY_RENDER_HPP_
#define RAY_RENDER_HPP_

#include <memory>
#include <vector>
#include <future>

#include <estl/vector.hpp>

#include "color.hpp"
#include "light.hpp"
#include "object.hpp"

namespace ray {
enum RenderStyle {
  SINGLE_PASS,
  HORIZONTAL_PASS,
  VERTICAL_PASS,
  SCATTER_PASS,
  MULTI_THREAD
};

void Render(const std::vector<std::unique_ptr<Object>>& objs,
            const std::vector<std::unique_ptr<Light>>& lights, unsigned width,
            unsigned height, double fov, RenderStyle style = SINGLE_PASS,
            int passes = 1);

void RenderSinglePass(const double& scale, const double& aspect,
                      const unsigned& width, const unsigned& height,
                      const std::vector<std::unique_ptr<Object>>& objs,
                      const std::vector<std::unique_ptr<Light>>& lights);
void RenderHorizontalPass(const double& scale, const double& aspect,
                          const unsigned& width, const unsigned& height,
                          const std::vector<std::unique_ptr<Object>>& objs,
                          const std::vector<std::unique_ptr<Light>>& lights);
void RenderVerticalPass(const double& scale, const double& aspect,
                        const unsigned& width, const unsigned& height,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights);
void RenderScatterPass(const double& scale, const double& aspect,
                       const unsigned& width, const unsigned& height,
                       const std::vector<std::unique_ptr<Object>>& objs,
                       const std::vector<std::unique_ptr<Light>>& lights,
                       const unsigned passes);
void RenderMultiThreadPass(const double& scale, const double& aspect,
                           const unsigned& width, const unsigned& height,
                           const std::vector<std::unique_ptr<Object>>& objs,
                           const std::vector<std::unique_ptr<Light>>& lights,
                           const unsigned passes);
std::vector<Color> RenderThread(const double& scale, const double& aspect,
                  const unsigned& width, const unsigned& height,
                  const std::vector<std::unique_ptr<Object>>& objs,
                  const std::vector<std::unique_ptr<Light>>& lights,
                  const unsigned& start, const unsigned& end);

Color RenderPixel(const double& scale, const double& aspect,
                  const unsigned& width, const unsigned& height,
                  const std::vector<std::unique_ptr<Object>>& objs,
                  const std::vector<std::unique_ptr<Light>>& lights,
                  const unsigned& row, const unsigned& col);

Color CastRay(const estl::vector::Vector<double, 3>& start,
              const estl::vector::Vector<double, 3>& dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              const std::vector<std::unique_ptr<Light>>& lights,
              unsigned depth = 0, const Object* caster = NULL);

Color Reflect(const estl::vector::Vector<double, 3>& point,
              const estl::vector::Vector<double, 3>& normal,
              const estl::vector::Vector<double, 3> dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              const std::vector<std::unique_ptr<Light>>& lights, double depth,
              const Object* obj);

bool TraceRay(const estl::vector::Vector<double, 3>& start,
              const estl::vector::Vector<double, 3>& dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              IntersectData& inter, bool base_ray = true);
}  // namespace ray

#endif  // RAY_RENDER_HPP_
