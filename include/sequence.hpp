#include "basic/vector.hpp"

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"

void action_1(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights, double& t);
void action_7(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights, double& t);
