#include <iostream>

#ifdef GRAPHICS
#include "entis.h"
#endif

#define WIDTH 400
#define HEIGHT 400
#define SAMPLES 1

#include "basic/vector.hpp"

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

void scene() {
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  // SELECT SAMPLE IMAGE
  // scene_1(objs, lights);
  // scene_2(objs, lights);
  // scene_3(objs, lights);
  // scene_4(objs, lights);
  // scene_5(objs, lights);
  // scene_5(objs, lights);
  scene_6(objs, lights);

#ifdef GRAPHICS
  entis_init("Ray", WIDTH, HEIGHT, 0, NULL);
  entis_clear();
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::ESTL,
              ray::SCATTER_PASS, 8);
  // Uncomment this for faster preformance, but no live preview.
  // ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::ESTL,
  //             ray::MULTI_THREAD, 8);
  entis_wait_button();
  entis_term();
#else
  ray::Render(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
              ray::MULTI_THREAD, 8);
#endif
}

void seq1() {
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;

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
  int id = 0;
  for (double x = -10.0; x <= 10.0; x += 0.33) {
    lights.back()->Move({x, 5.0, 7.0});
    ray::RenderSequence(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
                        ray::MULTI_THREAD, 8, "seq1/%05dimg.png", id);
    id++;
  }
  for (double x = 10.0; x >= -10.0; x -= 0.33) {
    lights.back()->Move({x, 5.0, 7.0});
    ray::RenderSequence(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
                        ray::MULTI_THREAD, 8, "seq1/%05dimg.png", id);
    id++;
  }
}

void seq2() {
  std::vector<std::unique_ptr<ray::Object>> objs;
  std::vector<std::unique_ptr<ray::Light>> lights;
  ray::Material mirror({1.0, 1.0, 1.0}, 80.0, 1.0);
  ray::Material mat({1.0, 1.0, 1.0}, 80.0, 0.0);

  lights.push_back(ray::MakeAreaLight({0, 0, 0}, {0.0, -1.0, 0.0}, 1.0, 1.0,
                                      SAMPLES, {1.0, 0.0, 0.0}));
  lights.push_back(ray::MakeAreaLight({0.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {0.0, 1.0, 0.0}));
  lights.push_back(ray::MakeAreaLight({0.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, 1.0,
                                      1.0, SAMPLES, {0.0, 0.0, 1.0}));

  objs.push_back(ray::GeneratePlane({0, -2, 0}, {0, 1, 0}, mat));
  objs.push_back(ray::GeneratePlane({0, 0, 15}, {0, 0, -1}, mat));

  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(-3, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mat));
  objs.back()->Translate(3, 0, 10);
  objs.push_back(ray::GenerateSphere(1, mirror));
  objs.back()->Translate(0, 0, 10);

  int id = 0;
  for (double t = 0; t <= 6.0 * M_PI; t += M_PI / 60) {
    lights[0]->Move({5 * cos(t / 3.0), 5.0, 5 * sin(t / 3.0) + 7.0});
    lights[1]->Move(
        {5 * cos(((2.0/3.0))*t + (2 * M_PI / 3)), 5.0, 5 * sin(((2.0/3.0)*t) + (2 * M_PI / 3)) + 7.0});
    lights[2]->Move(
        {5 * cos(t + (4 * M_PI / 3)), 5.0, 5 * sin(t + (4 * M_PI / 3)) + 7.0});
    ray::RenderSequence(objs, lights, WIDTH, HEIGHT, M_PI / 4.0, ray::PNG,
                        ray::MULTI_THREAD, 8, "seq2/%05dimg.png", id);
    id++;
  }
}

int main(int argc, char const* argv[]) {
  srand(time(NULL));

  // To Render a single image
  // scene();
  // To Render a series of images
  // seq1();
  seq2();
  return 0;
}
