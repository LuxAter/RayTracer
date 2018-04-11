#include "render.hpp"

#include <entis/entis.h>

#include <memory>
#include <vector>

#include <estl/vector.hpp>

#include "light.hpp"
#include "object.hpp"

#include <iostream>

using namespace estl::vector;

unsigned max_depth = 5;
Vector<double, 3> eye, coi, up;
ray::Color background_color = {0.2, 0.2, 0.3};

void ray::Render(const std::vector<std::unique_ptr<Object>>& objs,
                 const std::vector<std::unique_ptr<Light>>& lights,
                 unsigned width, unsigned height, double fov) {
  double scale = tan(fov / 2.0);
  double aspect = width / static_cast<double>(height);
  int cnt = 0;
  std::cout << "Rendering..." << std::flush;
  for (unsigned j = 0; j < height; j += 1) {
    for (unsigned i = 0; i < width; i += 1) {
      cnt++;
      double x =
          (2 * (i + 0.5) / static_cast<double>(width) - 1) * aspect * scale;
      double y = (1 - 2 * (j + 0.5) / static_cast<double>(height)) * scale;
      Vector<double, 3> dir({x, y, 1});
      dir = normalize(dir);
      Color col = CastRay({0, 0, 0}, dir, objs, lights);
      entis_set_color_drgb(col.r, col.g, col.b);
      entis_point(i, j);
    }
  }
  std::cout << "[DONE]\n";
}

ray::Color ray::CastRay(const estl::vector::Vector<double, 3>& start,
                        const estl::vector::Vector<double, 3>& dir,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights,
                        unsigned depth) {
  if (depth >= max_depth) {
    return background_color;
  }
  Intersect inter;
  if (TraceRay(start, dir, objs, inter) == true) {
    Color color = {0, 0, 0};
    for (auto& it : lights) {
      Vector<double, 3> light_dir, light_intensity, reflection;
      Intersect isec;
      it->Illuminate(inter.point, light_dir, light_intensity, isec.t_near);
      if (dot(inter.normal, light_dir) < 0 && dot(inter.normal, dir) < 0) {
        inter.normal *= -1;
      }
      reflection =
          light_dir - (2 * dot(inter.normal, light_dir) * inter.normal);
      double ambient = 0.2;
      double diffuse = 0.5 * std::max(0.0, dot(inter.normal, light_dir));
      double specular =
          0.3 *
          pow(std::max(0.0, dot(normalize(start - inter.point), reflection)),
              inter.mat.specular_exp);
      if(dot(dir, inter.normal) * dot(light_dir, inter.normal) < 0){
        diffuse = 0;
      }
      color = color + ((ambient * (inter.mat.ambient * it->color)) +
                       (diffuse * (inter.mat.diffuse * it->color)) +
                       (specular * (inter.mat.specular * it->color)));
    }
    return color;
  }
  return background_color;
}

bool ray::TraceRay(const estl::vector::Vector<double, 3>& start,
                   const estl::vector::Vector<double, 3>& dir,
                   const std::vector<std::unique_ptr<Object>>& objs,
                   ray::Intersect& inter) {
  inter.t_near = INFINITY;
  inter.obj = NULL;
  for (std::vector<std::unique_ptr<Object>>::const_iterator iter = objs.begin();
       iter != objs.end(); ++iter) {
    Intersect it;
    it.t_near = INFINITY;
    if ((*iter)->Intersect(start, dir, it) == true &&
        it.t_near < inter.t_near) {
      inter = it;
      inter.obj = iter->get();
    }
  }
  return (inter.obj != NULL);
}
