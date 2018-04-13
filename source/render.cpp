#include "render.hpp"

#include <entis/entis.h>

#include <chrono>
#include <memory>
#include <thread>
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
double bias = 0.001;
Vector<double, 3> eye, coi, up;
ray::Color background_color = {0.2, 0.2, 0.3};

void ray::Render(const std::vector<std::unique_ptr<Object>>& objs,
                 const std::vector<std::unique_ptr<Light>>& lights,
                 unsigned width, unsigned height, double fov, RenderStyle style,
                 int passes) {
  auto start = std::chrono::high_resolution_clock::now();
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
    case MULTI_THREAD: {
      RenderMultiThreadPass(scale, aspect, width, height, objs, lights, passes);
      break;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto diff = end - start;
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
  auto milli =
      std::chrono::duration_cast<std::chrono::milliseconds>(diff - sec);
  auto micro =
      std::chrono::duration_cast<std::chrono::microseconds>(diff - sec - milli);
  auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
      diff - sec - milli - micro);
  std::cout << sec.count() << "s " << milli.count() << "ms " << micro.count()
            << "μs " << nano.count() << "ns\n";
  entis_update();
}

void ray::RenderSinglePass(const double& scale, const double& aspect,
                           const unsigned& width, const unsigned& height,
                           const std::vector<std::unique_ptr<Object>>& objs,
                           const std::vector<std::unique_ptr<Light>>& lights) {
  for (unsigned i = 0; i < height; i += 1) {
    for (unsigned j = 0; j < width; j += 1) {
      Color color =
          RenderPixel(scale, aspect, width, height, objs, lights, i, j);
      entis_set_color_drgb(color.r, color.g, color.b);
      entis_point(j, i);
    }
  }
}
void ray::RenderHorizontalPass(
    const double& scale, const double& aspect, const unsigned& width,
    const unsigned& height, const std::vector<std::unique_ptr<Object>>& objs,
    const std::vector<std::unique_ptr<Light>>& lights) {
  for (unsigned i = 0; i < height; ++i) {
    for (unsigned j = 0; j < width; ++j) {
      Color color =
          RenderPixel(scale, aspect, width, height, objs, lights, i, j);
      entis_set_color_drgb(color.r, color.g, color.b);
      entis_point(j, i);
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
      Color color =
          RenderPixel(scale, aspect, width, height, objs, lights, i, j);
      entis_set_color_drgb(color.r, color.g, color.b);
      entis_point(j, i);
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
      Color color =
          RenderPixel(scale, aspect, width, height, objs, lights, i, j);
      entis_set_color_drgb(color.r, color.g, color.b);
      entis_point(j, i);
    }
    entis_update();
  }
}

void ray::RenderMultiThreadPass(
    const double& scale, const double& aspect, const unsigned& width,
    const unsigned& height, const std::vector<std::unique_ptr<Object>>& objs,
    const std::vector<std::unique_ptr<Light>>& lights, const unsigned passes) {
  // std::vector<std::thread> threads;
  std::vector<std::future<std::vector<Color>>> threads;
  double ppt = height / passes;
  // unsigned position = 0;
  for (unsigned id = 0; id < passes - 1; ++id) {
    threads.push_back(
        std::async(std::launch::async, RenderThread, std::ref(scale),
                   std::ref(aspect), std::ref(width), std::ref(height),
                   std::ref(objs), std::ref(lights), ppt * id, ppt * (id +
                   1)));
  }
  std::vector<Color> colors = RenderThread(scale, aspect, width, height, objs,
                                           lights, ppt * (passes - 1), height);
  unsigned pos = 0;
  for (unsigned i = ppt * (passes - 1); i < height; ++i) {
    for (unsigned j = 0; j < width; ++j) {
      Color color = colors[pos];
      entis_set_color_drgb(color.r, color.g, color.b);
      entis_point(j, i);
      pos++;
    }
  }
  for (unsigned id = 0; id < passes - 1; ++id) {
    pos = 0;
    colors = threads[id].get();
    for (unsigned i = ppt * id; i < ppt * (id + 1); ++i) {
      for (unsigned j = 0; j < width; ++j) {
        Color color = colors[pos];
        entis_set_color_drgb(color.r, color.g, color.b);
        entis_point(j, i);
        pos++;
      }
    }
  }
  // for (unsigned k = passes - 1, i = 0; k < pixels && i < colors.size();
  //      k += passes, ++i) {
  //   entis_set_color_drgb(colors[i].r, colors[i].g, colors[i].b);
  //   unsigned y = k % width;
  //   unsigned x = (k - i) / width;
  //   entis_point(x, y);
  // }
  // for (unsigned id = 0; id < passes - 1; ++id) {
  //   colors = threads[id].get();
  //   for (unsigned k = id, i = 0; k < pixels && i < colors.size();
  //        k += passes, ++i) {
  //     entis_set_color_drgb(colors[i].r, colors[i].g, colors[i].b);
  //     unsigned y = k % width;
  //     unsigned x = (k - i) / width;
  //     entis_point(x, y);
  //   }
  // threads[id].join();
  // }
  entis_update();
}
std::vector<ray::Color> ray::RenderThread(
    const double& scale, const double& aspect, const unsigned& width,
    const unsigned& height, const std::vector<std::unique_ptr<Object>>& objs,
    const std::vector<std::unique_ptr<Light>>& lights, const unsigned& start,
    const unsigned& end) {
  std::vector<Color> colors;
  for (unsigned i = start; i < end; ++i) {
    for (unsigned j = 0; j < width; ++j) {
      colors.push_back(
          RenderPixel(scale, aspect, width, height, objs, lights, i, j));
    }
  }
  return colors;
}

ray::Color ray::RenderPixel(const double& scale, const double& aspect,
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
  return color;
}

ray::Color ray::CastRay(const estl::vector::Vector<double, 3>& start,
                        const estl::vector::Vector<double, 3>& dir,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights,
                        unsigned depth, const Object* caster) {
  if (depth >= max_depth) {
    return background_color;
  }
  IntersectData inter;
  if (TraceRay(start, dir, objs, inter) == true && inter.obj != caster) {
    Color color;
    for (auto& it : lights) {
      Vector<double, 3> light_dir, light_intensity, reflection;
      IntersectData isec;
      it->Illuminate(inter.point, light_dir, light_intensity, isec.t_near);
      bool vis = !TraceRay(inter.point + (light_dir * -bias), light_dir * -1,
                           objs, isec, false);
      if (isec.obj == inter.obj) {
        vis = true;
      }
      if (dot(inter.normal, light_dir) < 0 && dot(inter.normal, dir) < 0) {
        inter.normal *= -1.0;
      }
      reflection =
          light_dir - (2 * dot(inter.normal, light_dir) * inter.normal);
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
    if (inter.mat.reflectivity != 0.0) {
      color = Combine(color, 1.0 - inter.mat.reflectivity,
                      Reflect(inter.point, inter.normal, dir, objs, lights,
                              depth, inter.obj),
                      inter.mat.reflectivity);
    }
    return color;
  }
  return background_color;
}

ray::Color ray::Reflect(const estl::vector::Vector<double, 3>& point,
                        const estl::vector::Vector<double, 3>& normal,
                        const estl::vector::Vector<double, 3> dir,
                        const std::vector<std::unique_ptr<Object>>& objs,
                        const std::vector<std::unique_ptr<Light>>& lights,
                        double depth, const Object* obj) {
  estl::vector::Vector<double, 3> reflection =
      dir - (2 * dot(normal, dir) * normal);
  return CastRay(point + (reflection * bias), reflection, objs, lights,
                 depth + 1, obj);
}

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
