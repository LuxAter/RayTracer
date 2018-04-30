#include <iostream>

#ifdef GRAPHICS
#include "entis.h"
#endif

#define WIDTH 1920
#define HEIGHT 1080

#include "basic/vector.hpp"

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"

#include "scenes.hpp"
#include "sequence.hpp"

void scene(void (*setup)(std::vector<std::unique_ptr<ray::Object>>&, std::vector<std::unique_ptr<ray::Light>>&)) {
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  setup(objs, lights);
#ifdef GRAPHICS
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::ESTL,
              ray::MULTI_THREAD, 8, "images/img.png");
  entis_wait_button();
  entis_term();
#else
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
              ray::MULTI_THREAD, 8, "images/img.png");
#endif
}

void sequence(void (*setup)(std::vector<std::unique_ptr<ray::Object>>&,
                            std::vector<std::unique_ptr<ray::Light>>&),
              void (*action)(std::vector<std::unique_ptr<ray::Object>>&,
                             std::vector<std::unique_ptr<ray::Light>>&,
                             double&),
              std::string fmt, double min, double max, int n_frame, int id = 0,
              int split = 1) {
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  setup(objs, lights);
  double step = fabs(max - min) / n_frame;
  int start_id = (n_frame / split) * id;
  int end_id = (n_frame / split) * (id + 1);
  for (double t = min + (start_id * step); t <= min + (end_id * step);
       t += step) {
    action(objs, lights, t);
    ray::RenderSequence(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
                        ray::MULTI_THREAD, 8, fmt, start_id);
    start_id++;
  }
}

int main(int argc, char const* argv[]) {
  srand(time(NULL));

  // To Render a single image
  // valid arguments are scene_1, scene_2, ..., scene_7
  // scene(scene_1);
  // To Render a series of images
  // valid arguments are scene_1, action_1 or scene_7, acition_7
  sequence(scene_1, action_1, "images/sequence2/%05d.png", 0, 6.0 * M_PI, 360,
           atoi(argv[1]), 12);
  return 0;
}
