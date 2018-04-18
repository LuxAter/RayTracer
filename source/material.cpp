#include "material.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "color.hpp"

#include <iostream>

ray::Material::Material() {}
ray::Material::Material(Color color, double spec, double reflect) {
  ambient = color;
  diffuse = color;
  specular = color;
  specular_exp = spec;
  optical_denisty = 1.0;
  dissolve = 1.0;
  reflectivity = reflect;
  illum = 1;
}
ray::Material::Material(std::string str) {
  std::istringstream iss(str);
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                  std::istream_iterator<std::string>{}};
  int flag = 0;
  double r, g, b;
  for (std::string& it : tokens) {
    if (flag == 0) {
      if (it == "newmtl") {
        flag = 1;
      } else if (it == "Ns") {
        flag = 2;
      } else if (it == "Ka") {
        flag = 6;
      } else if (it == "Kd") {
        flag = 7;
      } else if (it == "Ks") {
        flag = 8;
      } else if (it == "Ke") {
        flag = 9;
      } else if (it == "Ni") {
        flag = 3;
      } else if (it == "d") {
        flag = 4;
      } else if (it == "illum") {
        flag = 5;
      }
    } else if (flag == 1) {
      name = it;
      flag = 0;
    } else if (flag == 2) {
      specular_exp = stod(it);
      flag = 0;
    } else if (flag == 3) {
      optical_denisty = stod(it);
      flag = 0;
    } else if (flag == 4) {
      dissolve = stod(it);
      flag = 0;
    } else if (flag == 5) {
      illum = stoi(it);
      flag = 0;
    } else if (flag >= 6 && flag <= 9) {
      r = stod(it);
      flag += 4;
    } else if (flag >= 10 && flag <= 13) {
      g = stod(it);
      flag += 4;
    } else if (flag >= 14 && flag <= 17) {
      b = stod(it);
      if (flag == 14) {
        ambient = Color(r, g, b);
      } else if (flag == 15) {
        diffuse = Color(r, g, b);
      } else if (flag == 16) {
        specular = Color(r, g, b);
      } else if (flag == 17) {
        emissive = Color(r, g, b);
      }
      flag = 0;
    }
  }
}

std::map<std::string, ray::Material> ray::ParseMaterialFile(
    std::string file_path) {
  std::ifstream file(file_path);
  std::map<std::string, Material> materials;
  if (file.is_open()) {
    std::string current, line;
    bool reading = false;
    while (getline(file, line)) {
      if (line.compare(0, 6, "newmtl") == 0) {
        if (reading == true) {
          Material mat(current);
          materials[mat.name] = mat;
          current = std::string();
        } else {
          reading = true;
        }
      }
      if (reading == true) {
        current += line.substr(0, line.find('#', 0)) + ' ';
      }
    }
    Material mat(current);
    materials[mat.name] = mat;
    file.close();
  }
  return materials;
}

std::ostream& ray::operator<<(std::ostream& out, Material mat) {
  out << "Ka: " << mat.ambient << ", Kd: " << mat.diffuse
      << ", Ks: " << mat.specular << ", Ke: " << mat.emissive
      << ", Ns: " << mat.specular_exp << ", Ni: " << mat.optical_denisty
      << ", d: " << mat.dissolve << ", illum: " << mat.illum;
  return out;
}

bool ray::operator<(const Material& lhs, const Material& rhs) {
  return lhs.name < rhs.name;
}
