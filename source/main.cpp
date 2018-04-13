#include <iostream>

#include <entis/entis.h>

#define WIDTH 800
#define HEIGHT 800
#define RES 1

#include "material.hpp"
#include "object.hpp"
#include "light.hpp"
#include "render.hpp"

// using namespace estl::vector;

int main(int argc, char const* argv[]) {
  // std::vector<std::unique_ptr<ray::Object>> objs =
  // ray::ParseObjFile("./tree.obj");
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
  mat.specular = {0.0, 0.0, 0.0};
  mat.specular_exp = 76.8;
  mat.reflectivity = 0.5;

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
  // COOL
  lights.push_back(ray::MakePointLight({{20.0, 5.0, 9.0}}, {1.0, 0.0, 0.0}));
  lights.push_back(ray::MakePointLight({{-20.0, 5.0, 9.0}}, {0.0, 0.0, 1.0}));
  lights.push_back(ray::MakePointLight({{0.0, 10.0, 14.0}}, {0.0, 1.0, 0.0}));
  // lights.push_back(ray::MakePointLight({{5.0, 3.0, 5.0}}, {1.0, 1.0, 1.0}));
  objs.push_back(ray::GeneratePlane({0, -1, 0}, {0.0, 1.0, 0.0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 15}, {0.0, 0.0, -1.0}, mat));
  // objs.push_back(ray::GeneratePlane({0, 0, -5}, {0.0, 0.0, 1.0}, mat));
  objs.push_back(ray::GenerateSphere(1, mis));
  objs.back()->Translate(1, 2, 8.5);
  objs.push_back(ray::GenerateSphere(1, mis));
  objs.back()->Translate(-1, 0, 10);

  // objs.push_back(ray::GenerateSphere(50, refl));
  // objs.back()->Translate(0, 0, 70);
  // FALLOFF
  // lights.push_back(ray::MakePointLight({{0.0, -0.75, 10.0}}, {1.0, 1.0, 1.0}, 20));
  // objs.push_back(ray::GeneratePlane({0, -1, 0}, {0.0, 1.0, 0.0}, mat));
  
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  entis_set_color_drgb(1.0, 0.0, 1.0);
  // ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::VERTICAL_PASS, 1);
  // ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::SCATTER_PASS, 8);
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::MULTI_THREAD, 8);
  // entis_wait_event_type(ENTIS_KEY_RELEAS);
  entis_wait_button();
  entis_term();

  return 0;
}
