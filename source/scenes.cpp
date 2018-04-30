#include "scenes.hpp"

#include "basic/vector.hpp"

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"

#define SAMPLES 4

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

void scene_5(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);
  ray::Material grey({0.8, 0.8, 0.8}, 30.0, 0.0);
  lights.push_back(ray::MakePointLight({0.0, 0.0, 15.0}, {1.0, 1.0, 1.0}));
  objs.push_back(ray::GenerateTriangle({-0.5, -0.5, 10.0}, {0.0, 0.5, 10.0},
                                       {0.5, -0.5, 10.0}, mat));
}

void scene_6(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);
  ray::Material mirror({1.0, 1.0, 1.0}, 80.0, 0.75);
  lights.push_back(ray::MakePointLight({3.0, 10.0, 5.0}, {1.0, 1.0, 1.0}));
  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0, 1, 0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 20}, {0, 0, -1}, mat));
  std::vector<estl::base::Vec3d> verts;
  std::vector<unsigned> ind;
  int phi_count = 20;
  int theta_count = 40;
  for (double phi = 0; phi < M_PI; phi += M_PI / (double)phi_count) {
    for (double theta = 0; theta < 2.0 * M_PI;
         theta += 2.0 * M_PI / (double)theta_count) {
      verts.push_back({sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta)});
    }
  }
  for (unsigned i = 0; i < phi_count; i++) {
    for (unsigned j = 0; j < theta_count; j++) {
      int first = (i * theta_count) + j;
      int second = first + theta_count;
      ind.push_back(first);
      ind.push_back(second);
      ind.push_back(first + 1);
      ind.push_back(second);
      ind.push_back(second + 1);
      ind.push_back(first + 1);
    }
  }

  objs.push_back(ray::GenerateMesh(verts, ind, mat));
  objs.back()->Translate(0, 0, 10);
}

void scene_7(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights) {
  ray::Material mirror({1.0, 1.0, 1.0}, 80.0, 1.0);
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);

  lights.push_back(ray::MakePointLight({0.0, 5.0, 7.0}, {1.0, 1.0, 1.0}));

  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0, 1, 0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 15}, {0, 0, -1}, mat));

  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(-3, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(3, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mirror));
  objs.back()->Translate(0, 0, 10);
}
