#include <iostream>

#ifdef GRAPHICS
#include "entis.h"
#endif

#define WIDTH 500
#define HEIGHT 500
#define RES 1

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"


int main(int argc, char const* argv[]) {
  srand(time(NULL));
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);
  ray::Material surf({1.0, 1.0, 1.0}, 80.0, 0.0);
  ray::Material refl({1.0, 0.5, 1.0}, 30.0, 0.75);
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  lights.push_back(ray::MakePointLight({-5.0, 5.0, 20.0}, {1.0, 1.0, 0.0}));
  lights.push_back(ray::MakePointLight({5.0, 5.0, 20.0}, {1.0, 0.0, 1.0}));
  lights.push_back(ray::MakePointLight({0.0, 5.0, 5.0}, {0.0, 1.0, 1.0}));
  // lights.push_back(ray::MakeAreaLight({0.0, 10.0, 5.0}, {0.0, -1.0, 0.0}, 1.0, 1.0, 1, {1.0, 1.0, 1.0}));
  // lights.push_back(ray::MakeAreaLight({5.0, 10.0, 5.0}, {0.0, -1.0, 0.0}, 1.0, 1.0, 1, {1.0, 1.0, 1.0}));
  // lights.push_back(ray::MakeAreaLight({-5.0, 10.0, 5.0}, {0.0, -1.0, 0.0}, 1.0, 1.0, 1, {1.0, 1.0, 1.0}));
  // lights.push_back(ray::MakeAreaLight({5.0, 20.0, 10.0}, {1.0, 1.0, 1.0}));
  // lights.push_back(ray::MakePointLight({0.0, 5.0, 0.0}, {1.0, 1.0, 1.0}));
  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0.0, 1.0, 0.0}, refl));
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(0, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(2, 0, 15);
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(-2, -1, 15);

#ifdef GRAPHICS
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::ESTL,
              ray::SCATTER_PASS, 31);
  entis_wait_button();
  entis_term();
#else
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
              ray::MULTI_THREAD, 8);
#endif

  return 0;
}
