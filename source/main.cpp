#include <iostream>

#ifdef GRAPHICS
#include "entis.h"
#endif

#define WIDTH 800
#define HEIGHT 800
#define SAMPLES 4

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"

void scene_1(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mirror({1.0, 1.0, 1.0}, 80.0, 1.0);
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);

  lights.push_back(ray::MakeAreaLight({-10.0, 5.0, 7.0}, {0.0, -1.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {1.0, 0.0, 0.0}));
  lights.push_back(ray::MakeAreaLight({0.0, 5.0, 7.0}, {0.0, -1.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {0.0, 1.0, 0.0}));
  lights.push_back(ray::MakeAreaLight({10.0, 5.0, 7.0}, {0.0, -1.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {0.0, 0.0, 1.0}));

  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0, 1, 0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 15}, {0, 0, -1}, mat));

  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(-3, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(3, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mirror));
  objs.back()->Translate(0, 0, 10);
}

void scene_2(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mirror({1.0, 1.0, 1.0}, 80.0, 1.0);
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);

  lights.push_back(ray::MakeAreaLight({0.0, 50.0, 10.0}, {0.0, -1.0, 0.0}, 10.0,
                                      10.0, SAMPLES, {1.0, 1.0, 1.0}));

  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0, 1, 0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 20}, {0, 0, -1}, mirror));
  objs.push_back(ray::GeneratePlane({0, 0, -.1}, {0, 0, 1}, mirror));

  for (unsigned i = 0; i <= 20; i += 5) {
    ray::Material a({ray::Rand(0, 1), ray::Rand(0, 1), ray::Rand(0, 1)},
                    ray::Rand(10, 100), ray::Rand(0.0, 0.2));
    ray::Material b({ray::Rand(0, 1), ray::Rand(0, 1), ray::Rand(0, 1)},
                    ray::Rand(10, 100), ray::Rand(0.0, 0.2));
    objs.push_back(ray::GenerateSphere(1, a));
    objs.back()->Translate(-3, 0, i);
    objs.push_back(ray::GenerateSphere(1, b));
    objs.back()->Translate(3, 0, i);
  }
}

void scene_3(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mirror({1.0, 1.0, 1.0}, 80.0, 1.0);
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);

  lights.push_back(ray::MakeAreaLight({10.0, 2.0, 20.0}, {-1.0, 0.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {1.0, 0.0, 0.0}));
  lights.push_back(ray::MakeAreaLight({-10.0, 2.0, 20.0}, {1.0, 0.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {0.0, 0.0, 1.0}));
  objs.push_back(ray::GeneratePlane({0, -1, 0}, {0, 1, 0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 60}, {0, 0, -1}, mat));

  for (unsigned i = 0; i <= 50; i += 5) {
    objs.push_back(ray::GenerateSphere(1, mat));
    objs.back()->Translate(-3, 0, i);
    objs.push_back(ray::GenerateSphere(1, mat));
    objs.back()->Translate(3, 0, i);
  }
}

void scene_4(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);
  ray::Material grey({0.8, 0.8, 0.8}, 30.0, 0.0);

  lights.push_back(ray::MakePointLight({0.0, 0.0, 50}, {1.0, 1.0, 1.0}, 1500));
  objs.push_back(ray::GeneratePlane({0, 0, 51}, {0, 0, -1}, mat));
  objs.push_back(ray::GenerateSphere(2, grey));
  objs.back()->Translate(4, 4, 51);
  objs.push_back(ray::GenerateSphere(2, grey));
  objs.back()->Translate(8, 0, 51);
  objs.push_back(ray::GenerateSphere(2, grey));
  objs.back()->Translate(-4, 4, 51);
  objs.push_back(ray::GenerateSphere(2, grey));
  objs.back()->Translate(0, -8, 51);
}


int main(int argc, char const* argv[]) {
  srand(time(NULL));
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;

  // SELECT SAMPLE IMAGE
  scene_1(objs, lights);
  // scene_2(objs, lights);
  // scene_3(objs, lights);
  // scene_4(objs, lights);

#ifdef GRAPHICS
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::ESTL,
              ray::SCATTER_PASS, 31);
  // Uncomment this for faster preformance, but no live preview.
  // ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::ESTL,
  //             ray::MULTI_THREAD, 8);
  entis_wait_button();
  entis_term();
#else
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
              ray::MULTI_THREAD, 8);
#endif

  return 0;
}
