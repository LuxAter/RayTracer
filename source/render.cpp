#include "render.hpp"

#include <entis/entis.h>

#include <memory>
#include <vector>

#include <estl/vector.hpp>

#include "light.hpp"
#include "object.hpp"

#include <iostream>

#define SINGLEPASS

using namespace estl::vector;

typedef estl::vector::Vector<double, 3> Vec3;
typedef estl::vector::Vector<double, 4> Vec4;

unsigned max_depth = 5;
double bias = 0.01;
Vector<double, 3> eye, coi, up;
ray::Color background_color = {0.2, 0.2, 0.3};

void ray::Render(const std::vector<std::unique_ptr<Object>>& objs,
                 const std::vector<std::unique_ptr<Light>>& lights,
                 unsigned width, unsigned height, double fov, RenderStyle style,
                 int passes) {
  double scale = tan(fov / 2.0);
  double aspect = width / static_cast<double>(height);
  switch (style) {
    case SINGLE_PASS: {
      RenderSinglePass(scale, aspect, width, height, objs, lights);
      break;
    }
    case HORIZONTAL_PASS: {
      RenderHorizontalPass(scale, aspect, width, height, objs, lights);
      break;
    }
    case VERTICAL_PASS: {
      RenderVerticalPass(scale, aspect, width, height, objs, lights);
      break;
    }
    case SCATTER_PASS: {
      RenderScatterPass(scale, aspect, width, height, objs, lights, passes);
      break;
    }
  }
  entis_update();
}

void ray::RenderSinglePass(const double& scale, const double& aspect,
                           const unsigned& width, const unsigned& height,
                           const std::vector<std::unique_ptr<Object>>& objs,
                           const std::vector<std::unique_ptr<Light>>& lights) {
  for (unsigned i = 0; i < height; i += 1) {
    for (unsigned j = 0; j < width; j += 1) {
      RenderPixel(scale, aspect, width, height, objs, lights, i, j);
    }
  }
}
void ray::RenderHorizontalPass(
    const double& scale, const double& aspect, const unsigned& width,
    const unsigned& height, const std::vector<std::unique_ptr<Object>>& objs,
    const std::vector<std::unique_ptr<Light>>& lights) {
  for (unsigned i = 0; i < height; ++i) {
    for (unsigned j = 0; j < width; ++j) {
      RenderPixel(scale, aspect, width, height, objs, lights, i, j);
    }
    entis_update();
  }
}
void ray::RenderVerticalPass(
    const double& scale, const double& aspect, const unsigned& width,
    const unsigned& height, const std::vector<std::unique_ptr<Object>>& objs,
    const std::vector<std::unique_ptr<Light>>& lights) {
  for (unsigned j = 0; j < width; ++j) {
    for (unsigned i = 0; i < height; ++i) {
      RenderPixel(scale, aspect, width, height, objs, lights, i, j);
    }
    entis_update();
  }
}
void ray::RenderScatterPass(const double& scale, const double& aspect,
                            const unsigned& width, const unsigned& height,
                            const std::vector<std::unique_ptr<Object>>& objs,
                            const std::vector<std::unique_ptr<Light>>& lights,
                            const unsigned passes) {
  unsigned pixels = height * width;
  for (unsigned offset = 0; offset < passes; ++offset) {
    for (unsigned k = offset; k < pixels; k += passes) {
      unsigned i = k % width;
      unsigned j = (k - i) / width;
      RenderPixel(scale, aspect, width, height, objs, lights, i, j);
    }
    entis_update();
  }
}

void ray::RenderPixel(const double& scale, const double& aspect,
                      const unsigned& width, const unsigned& height,
                      const std::vector<std::unique_ptr<Object>>& objs,
                      const std::vector<std::unique_ptr<Light>>& lights,
                      const unsigned& row, const unsigned& col) {
  double x =
      (2.0 * (col + 0.5) / static_cast<double>(width) - 1.0) * aspect * scale;
  double y = (1.0 - 2.0 * (row + 0.5) / static_cast<double>(height)) * scale;
  Vector<double, 3> dir({x, y, 1});
  dir = normalize(dir);
  Color color = CastRay({0, 0, 0}, dir, objs, lights);
  color.Clamp();
  entis_set_color_drgb(color.r, color.g, color.b);
  entis_point(col, row);
}

ray::Color ray::CastRay(const estl::vector::Vector<double, 3>& start,
                        const estl::vector::Vector<double, 3>& dir,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights,
                        unsigned depth) {
  if (depth >= max_depth) {
    return background_color;
  }
  IntersectData inter;
  if (TraceRay(start, dir, objs, inter) == true) {
    Color color = {0.0, 0.0, 0.0};
    switch (inter.obj->type) {
      case Object::DIFFUSE: {
        color = Diffuse(inter, start, dir, objs, lights);
        break;
      }
      case Object::REFLECTION: {
        break;
      }
      case Object::REFLECTION_REFRACTION: {
        break;
      }
    }
    return color;
  }
  return background_color;
}

ray::Color ray::Diffuse(IntersectData& inter,
                        const estl::vector::Vector<double, 3>& start,
                        const estl::vector::Vector<double, 3>& dir,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights) {
  Color color;
  for (auto& it : lights) {
    Vector<double, 3> light_dir, light_intensity, reflection;
    IntersectData isec;
    it->Illuminate(inter.point, light_dir, light_intensity, isec.t_near);
    bool vis = !TraceRay(inter.point + (inter.normal * bias), light_dir * -1,
                         objs, isec, false);
    // vis = true;
    if (isec.obj == inter.obj) {
      vis = true;
    }
    if (dot(inter.normal, light_dir) < 0 && dot(inter.normal, dir) < 0) {
      inter.normal *= -1.0;
    }
    reflection = light_dir - (2 * dot(inter.normal, light_dir) * inter.normal);
    double ambient = 0.2;
    double diffuse = 0.5 * vis * std::max(0.0, dot(inter.normal, light_dir));
    double specular =
        0.3 * vis *
        pow(std::max(0.0, dot(normalize(start - inter.point), reflection)),
            inter.mat.specular_exp);
    if (dot(dir, inter.normal) * dot(light_dir, inter.normal) < 0) {
      diffuse = 0;
    }
    color +=
        Color((ambient * (inter.mat.ambient.Vector() * light_intensity)) +
              (diffuse * (inter.mat.diffuse.Vector() * light_intensity)) +
              (specular * (inter.mat.specular.Vector() * light_intensity)));
  }
  return color;
}
//       if (dot(inter.normal, light_dir) < 0 && dot(inter.normal, dir) < 0) {
//         inter.normal *= -1;
//       }
//       reflection =
//           light_dir - (2 * dot(inter.normal, light_dir) * inter.normal);
//       double ambient = 0.2;
//       double diffuse = 0.5 * vis * std::max(0.0, dot(inter.normal,
//       light_dir)); double specular =
//           0.3 * vis *
//           pow(std::max(0.0, dot(normalize(start - inter.point),
//           reflection)),
//               inter.mat.specular_exp);
//       if (dot(dir, inter.normal) * dot(light_dir, inter.normal) < 0) {
//         diffuse = 0;
//       }
//       color = color + ((ambient * (inter.mat.ambient * it->color)) +
//                        (diffuse * (inter.mat.diffuse * it->color)) +
//                        (specular * (inter.mat.specular * it->color)));

bool ray::TraceRay(const estl::vector::Vector<double, 3>& start,
                   const estl::vector::Vector<double, 3>& dir,
                   const std::vector<std::unique_ptr<Object>>& objs,
                   IntersectData& inter, bool base_ray) {
  inter.obj = NULL;
  for (std::vector<std::unique_ptr<Object>>::const_iterator iter = objs.begin();
       iter != objs.end(); ++iter) {
    IntersectData it;
    it.t_near = INFINITY;
    if ((*iter)->Intersect(start, dir, it) == true &&
        it.t_near < inter.t_near) {
      inter = it;
      inter.obj = iter->get();
    }
  }
  return (inter.obj != NULL);
}
// ray::Color ray::CastRay(const estl::vector::Vector<double, 3>& start,
//                         const estl::vector::Vector<double, 3>& dir,
//                         const std::vector<std::unique_ptr<Object>>& objs,
//                         const std::vector<std::unique_ptr<Light>>& lights,
//                         unsigned depth) {
//   if (depth >= max_depth) {
//     return background_color;
//   }
//   Intersect inter;
//   if (TraceRay(start, dir, objs, inter) == true) {
//     Color color = {0, 0, 0};
//     std::cout << "\n";
//     for (auto& it : lights) {
//       Vector<double, 3> light_dir, light_intensity, reflection;
//       Intersect isec;
//       it->Illuminate(inter.point, light_dir, light_intensity, isec.t_near);
//       bool vis = true;
//       if (inter.obj->type == Object::SPHERE) {
//         vis = !TraceRay(inter.point + (inter.normal * bias), light_dir *
//         -1,
//                         objs, isec);
//         std::cout << vis;
//       } else {
//         // vis = !TraceRay(inter.point, light_dir * -1, objs, isec);
//       }
//       // vis = true;
//       if (dot(inter.normal, light_dir) < 0 && dot(inter.normal, dir) < 0) {
//         inter.normal *= -1;
//       }
//       reflection =
//           light_dir - (2 * dot(inter.normal, light_dir) * inter.normal);
//       double ambient = 0.2;
//       double diffuse = 0.5 * vis * std::max(0.0, dot(inter.normal,
//       light_dir)); double specular =
//           0.3 * vis *
//           pow(std::max(0.0, dot(normalize(start - inter.point),
//           reflection)),
//               inter.mat.specular_exp);
//       if (dot(dir, inter.normal) * dot(light_dir, inter.normal) < 0) {
//         diffuse = 0;
//       }
//       color = color + ((ambient * (inter.mat.ambient * it->color)) +
//                        (diffuse * (inter.mat.diffuse * it->color)) +
//                        (specular * (inter.mat.specular * it->color)));
//     }
//     return color;
//   }
//   return background_color;
// }
//
// bool ray::TraceRay(const estl::vector::Vector<double, 3>& start,
//                    const estl::vector::Vector<double, 3>& dir,
//                    const std::vector<std::unique_ptr<Object>>& objs,
//                    ray::Intersect& inter) {
//   inter.obj = NULL;
//   for (std::vector<std::unique_ptr<Object>>::const_iterator iter =
//   objs.begin();
//        iter != objs.end(); ++iter) {
//     Intersect it;
//     it.t_near = INFINITY;
//     if ((*iter)->Intersect(start, dir, it) == true &&
//         it.t_near < inter.t_near) {
//       inter = it;
//       inter.obj = iter->get();
//     }
//   }
//   return (inter.obj != NULL);
// }
