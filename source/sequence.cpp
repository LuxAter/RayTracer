#include "sequence.hpp"

#include "basic/vector.hpp"

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"

void action_1(std::vector<std::unique_ptr<ray::Object>>& objs,
              std::vector<std::unique_ptr<ray::Light>>& lights, double& t) {
  lights[0]->Move({5 * cos(t / 3.0), 5.0, 5 * sin(t / 3.0) + 7.0});
  lights[1]->Move({5 * cos(((2.0 / 3.0)) * t + (2 * M_PI / 3)), 5.0,
                   5 * sin(((2.0 / 3.0) * t) + (2 * M_PI / 3)) + 7.0});
  lights[2]->Move(
      {5 * cos(t + (4 * M_PI / 3)), 5.0, 5 * sin(t + (4 * M_PI / 3)) + 7.0});
}
void action_7(std::vector<std::unique_ptr<ray::Object>>& objs,
              std::vector<std::unique_ptr<ray::Light>>& lights, double& t) {
  lights.back()->Move({t, 5.0, 7.0});
}
