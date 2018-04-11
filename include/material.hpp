#ifndef RAY_MATERIAL_HPP_
#define RAY_MATERIAL_HPP_

#include <string>
#include <sstream>
#include <map>

#include "color.hpp"

namespace ray {
  class Material{
    public:
      Material();
      Material(std::string str);

      std::string name;
      Color ambient, diffuse, specular, emissive; 
      double specular_exp, optical_denisty, dissolve;
      int illum;
  };

  std::map<std::string, Material> ParseMaterialFile(std::string file_path);
  std::ostream& operator<<(std::ostream& out, Material mat);
  bool operator<(const Material& lhs, const Material& rhs);
} // namespace ray

#endif  // RAY_MATERIAL_HPP_
