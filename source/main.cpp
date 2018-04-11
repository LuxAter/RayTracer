#include <iostream>

#include <entis/entis.h>

#define WIDTH 500
#define HEIGHT 500
#define RES 1

#include "material.hpp"
#include "object.hpp"
#include "light.hpp"
#include "render.hpp"

// using namespace estl::vector;

int main(int argc, char const* argv[]) {
  // std::vector<std::unique_ptr<ray::Object>> objs =
  // ray::ParseObjFile("./tree.obj");
  ray::Material mat;
  // Emerald=====
  // mat.ambient = {0.0215, 0.1745, 0.0215};
  // mat.diffuse = {0.07568, 0.61424, 0.07568};
  // mat.specular = {0.633, 0.727811, 0.633};
  // mat.specular_exp = 76.8;
  // Misc
  mat.ambient = {1.0, 1.0, 1.0};
  mat.diffuse = {1.0, 1.0, 1.0};
  mat.specular = {1.0, 1.0, 1.0};
  mat.specular_exp = 76.8;
  mat.emissive = {0.0, 0.0, 0.0};
  mat.optical_denisty = 1.0;
  mat.dissolve = 1.0;
  mat.illum = 1;
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{0.0, 0.0, 7.0}}, {1.0, 0.0, 0.0}, 10)));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{0.0, 0.0, 5.0}}, {0.0, 1.0, 0.0}, 10)));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{0.0, 0.0, 3.0}}, {0.0, 0.0, 1.0}, 10)));

  // lights.push_back(ray::MakePointLight({{5.0, 0.0, 10.0}}, {1.0, 0.0, 0.0}));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{5.0, 0.0, 10.0}}, {1.0, 0.0, 0.0}, 10)));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{-5.0, 0.0, 10.0}}, {0.0, 1.0, 0.0}, 10)));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{0.0, 5.0, 10.0}}, {0.0, 0.0, 1.0}, 10)));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{0, 100.0, -10}}, {1.0, 1.0, 1.0}, 10)));
  // lights.push_back(std::make_unique<ray::Light>(ray::Light({{0.0, 0.0, 2.0}}, {1.0, 1.0, 1.0}, 10)));
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(0, 0, 1);
  // objs.push_back(ray::GenerateSphere(0.25, mat));
  // objs.back()->Translate(-1, 1, 10);
  // objs.push_back(ray::GenerateSphere(0.25, mat));
  // objs.back()->Translate(1, 1, 10);
  // objs.push_back(ray::GenerateSphere(0.25, mat));
  // objs.back()->Translate(-1, -1, 10);
  // objs.push_back(ray::GenerateSphere(0.25, mat));
  // objs.back()->Translate(1, -1, 10);
  // objs.push_back(ray::GenerateSphere(100, mat));
  // objs.back()->Translate(0, 0, 25);
  // objs.push_back(ray::GeneratePlane({0,0, 20}, {0, 0, -1}, mat));
  // objs[0]->sphere = true;
  // objs[0]->sphere_mat = mat;
  // std::cout << "A\n";
  // for(auto& it : objs){
  //   std::cout << it.name << ": " << it.vertices.size() << "\n";
  //   for(auto& it : it.faces){
  //     std::cout << it.first.name << ": " << it.second.size() << "\n";
  //   }
  // }
  // objs[0]->Translate(0, 0, 20);
  // ray::SetLightColor({1.0, 1.0, 1.0});
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  entis_set_color_drgb(1.0, 0.0, 1.0);
  ray::Render(objs, lights, WIDTH, HEIGHT, 45.0);
  entis_wait_event_type(ENTIS_KEY_RELEASE);
  entis_term();

  return 0;
}
