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
  ray::Material mat, em, refl, mis;
  // Emerald=====
  em.ambient = {0.0215, 0.1745, 0.0215};
  em.diffuse = {0.07568, 0.61424, 0.07568};
  em.specular = {0.633, 0.727811, 0.633};
  em.specular_exp = 76.8;
  em.reflectivity = 0.0;
  // Misc========
  mis.ambient = {1.0, 1.0, 1.0};
  mis.diffuse = {1.0, 1.0, 1.0};
  mis.specular = {1.0, 1.0, 1.0};
  mis.specular_exp = 76.8;
  mis.reflectivity = 0.0;
  // Reflect=====
  refl.ambient = {1.0, 1.0, 1.0};
  refl.diffuse = {1.0, 1.0, 1.0};
  refl.specular = {1.0, 1.0, 1.0};
  refl.specular_exp = 76.8;
  refl.reflectivity = 1.0;
  // Mat========
  mat.ambient = {1.0, 1.0, 1.0};
  mat.diffuse = {1.0, 1.0, 1.0};
  mat.specular = {1.0, 1.0, 1.0};
  mat.specular_exp = 76.8;
  mat.reflectivity = 0.4;

  mis.emissive = {0.0, 0.0, 0.0};
  mis.optical_denisty = 1.0;
  mis.dissolve = 1.0;
  mis.illum = 1;
  refl.emissive = {0.0, 0.0, 0.0};
  refl.optical_denisty = 1.0;
  refl.dissolve = 1.0;
  refl.illum = 1;
  mat.emissive = {0.0, 0.0, 0.0};
  mat.optical_denisty = 1.0;
  mat.dissolve = 1.0;
  mat.illum = 1;
  em.emissive = {0.0, 0.0, 0.0};
  em.optical_denisty = 1.0;
  em.dissolve = 1.0;
  em.illum = 1;
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  lights.push_back(ray::MakeAreaLight({0.0, 10.0, 5.0}, {0.0, -1.0, 0.0}, 1.0,
                                      1.0, 1, {1.0, 1.0, 1.0}));
  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0.0, 1.0, 0.0}, mis));
  objs.push_back(ray::GeneratePlane({0, 0, 15}, {0.0, 0.0, -1.0}, mis));
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(0, 0, 10);

#ifdef GRAPHICS
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  entis_set_color_drgb(1.0, 0.0, 1.0);
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
