#ifndef RAY_RENDER_HPP_
#define RAY_RENDER_HPP_

#include <future>
#include <memory>
#include <vector>

#include "basic/vector.hpp"

#include "color.hpp"
#include "light.hpp"
#include "object.hpp"

namespace ray {
enum RenderFormat { ESTL, PNG, ESTL_PNG };
enum RenderStyle {
  SINGLE_PASS,
  HORIZONTAL_PASS,
  VERTICAL_PASS,
  SCATTER_PASS,
  MULTI_THREAD
};

void RenderSequence(const std::vector<std::unique_ptr<Object>>& objs,
                    const std::vector<std::unique_ptr<Light>>& lights,
                    unsigned width, unsigned height, double fov,
                    RenderFormat fmt, RenderStyle style, int passes = 1,
                    std::string base_name = "img/%05dimg.png", int index = -1);
void Render(const std::vector<std::unique_ptr<Object>>& objs,
            const std::vector<std::unique_ptr<Light>>& lights, unsigned width,
            unsigned height, double fov, RenderFormat fmt = ESTL,
            RenderStyle style = SINGLE_PASS, int passes = 1,
            std::string file_path = "img.png");

void RenderSinglePass(const double& scale, const double& aspect,
                      const unsigned& width, const unsigned& height,
                      const std::vector<std::unique_ptr<Object>>& objs,
                      const std::vector<std::unique_ptr<Light>>& lights,
                      RenderFormat fmt);
void RenderHorizontalPass(const double& scale, const double& aspect,
                          const unsigned& width, const unsigned& height,
                          const std::vector<std::unique_ptr<Object>>& objs,
                          const std::vector<std::unique_ptr<Light>>& lights,
                          RenderFormat fmt);
void RenderVerticalPass(const double& scale, const double& aspect,
                        const unsigned& width, const unsigned& height,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights,
                        RenderFormat fmt);
void RenderScatterPass(const double& scale, const double& aspect,
                       const unsigned& width, const unsigned& height,
                       const std::vector<std::unique_ptr<Object>>& objs,
                       const std::vector<std::unique_ptr<Light>>& lights,
                       const unsigned passes, RenderFormat fmt);
void RenderMultiThreadPass(const double& scale, const double& aspect,
                           const unsigned& width, const unsigned& height,
                           const std::vector<std::unique_ptr<Object>>& objs,
                           const std::vector<std::unique_ptr<Light>>& lights,
                           const unsigned passes, RenderFormat fmt);
std::vector<Color> RenderThread(
    const double& scale, const double& aspect, const unsigned& width,
    const unsigned& height, const std::vector<std::unique_ptr<Object>>& objs,
    const std::vector<std::unique_ptr<Light>>& lights, const unsigned& start,
    const unsigned& end);

Color RenderPixel(const double& scale, const double& aspect,
                  const unsigned& width, const unsigned& height,
                  const std::vector<std::unique_ptr<Object>>& objs,
                  const std::vector<std::unique_ptr<Light>>& lights,
                  const unsigned& row, const unsigned& col);

Color CastRay(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              const std::vector<std::unique_ptr<Light>>& lights,
              unsigned depth = 0, const Object* caster = NULL);

Color Reflect(const estl::base::Vec3d& point, const estl::base::Vec3d& normal,
              const estl::base::Vec3d dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              const std::vector<std::unique_ptr<Light>>& lights, double depth,
              const Object* obj);

bool TraceRay(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
              const std::vector<std::unique_ptr<Object>>& objs,
              IntersectData& inter, bool base_ray = true);
double ShadowRay(const std::unique_ptr<Light>& light,
                 const IntersectData& inter,
                 const std::vector<std::unique_ptr<Object>>& objs,
                 estl::base::Vec3d& light_dir,
                 estl::base::Vec3d& light_intensity);
}  // namespace ray

#endif  // RAY_RENDER_HPP_
