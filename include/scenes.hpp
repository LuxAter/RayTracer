#include "basic/vector.hpp"

#include "light.hpp"
#include "material.hpp"
#include "object.hpp"
#include "png.hpp"
#include "render.hpp"

void scene_1(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
void scene_2(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
void scene_3(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
void scene_4(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
void scene_5(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
void scene_6(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
void scene_7(std::vector<std::unique_ptr<ray::Object>>& objs,
             std::vector<std::unique_ptr<ray::Light>>& lights);
