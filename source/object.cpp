#include "object.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <estl/vector.hpp>

#include "material.hpp"
#include "math.hpp"

#include <iostream>

using namespace estl::vector;

std::vector<std::string> split(const std::string& s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

ray::Object::Object() {
  mat_.fill_diagonal(1);
  mat_inv_.fill_diagonal(1);
}

ray::Object::~Object() {}

bool ray::Object::Intersect(const estl::vector::Vector<double, 3>& start,
                            const estl::vector::Vector<double, 3>& dir,
                            IntersectData& inter) {
  return false;
}

void ray::Object::Translate(double x, double y, double z) {
  estl::matrix::Matrix<double, 4, 4> trans;
  trans.fill_diagonal(1);
  trans(0, 3) = x;
  trans(1, 3) = y;
  trans(2, 3) = z;
  mat_ = trans * mat_;
  trans(0, 3) = -x;
  trans(1, 3) = -y;
  trans(2, 3) = -z;
  mat_inv_ = mat_inv_ * trans;
}

ray::Sphere::Sphere(const double& radius, Material mat)
    : Object(),
      material_(mat),
      radius_(radius),
      radius_square_(radius * radius) {
  this->name = "sphere";
}

bool ray::Sphere::Intersect(const estl::vector::Vector<double, 3>& start,
                            const estl::vector::Vector<double, 3>& dir,
                            IntersectData& inter) {
  Vector<double, 3> local_start(mat_inv_ * Vector<double, 4>(start, 1), 0);
  double t0, t1;
  double a = dot(dir, dir);
  double b = 2 * dot(dir, local_start);
  double c = dot(local_start, local_start) - radius_square_;
  if (Quadradic(a, b, c, t0, t1) == false) {
    return false;
  }
  if (t0 > t1) {
    std::swap(t0, t1);
  }
  if (t0 < 0) {
    if (t1 < 0) {
      return false;
    }
    inter.t_near = t1;
  } else {
    inter.t_near = t0;
  }
  inter.mat = material_;
  inter.point = start + (dir * inter.t_near);
  inter.normal = normalize(local_start + (dir * inter.t_near));
  return true;
}

ray::Plane::Plane(const estl::vector::Vector<double, 3>& origin,
                  const estl::vector::Vector<double, 3>& normal, Material mat)
    : Object(),
      material_(mat),
      constant_(-dot(origin, normal)),
      origin_(origin),
      normal_(normalize(normal)) {
  this->name = "plane";
}
bool ray::Plane::Intersect(const estl::vector::Vector<double, 3>& start,
                           const estl::vector::Vector<double, 3>& dir,
                           IntersectData& inter) {
  Vector<double, 3> local_start(mat_inv_ * Vector<double, 4>(start, 1), 0);
  double a = dot(normal_, dir);
  // if (a != 0) {
  //   double t = -(dot(normal_, local_start) + constant_) / a;
  //   if (t > 0 && t < inter.t_near) {
  //     inter.t_near = t;
  //     inter.mat = material_;
  //     inter.point = start + (dir * inter.t_near);
  //     inter.normal = normal_;
  //     return true;
  //   }
  // }
  // return false;
  double b = constant_ - dot(normal_, local_start);
  if (a == 0) {
    return false;
  } else if (b / a < 0) {
    return false;
  }
  inter.t_near = b / a;
  inter.mat = material_;
  inter.point = start + (dir * inter.t_near);
  inter.normal = normal_;
  return true;
}

ray::Circle::Circle(const estl::vector::Vector<double, 3>& origin,
                    const estl::vector::Vector<double, 3>& normal,
                    const double& radius, Material mat)
    : Object(),
      material_(mat),
      constant_(-dot(origin, normal)),
      radius_(radius),
      origin_(origin),
      normal_(normalize(normal)) {
  this->name = "plane";
}
bool ray::Circle::Intersect(const estl::vector::Vector<double, 3>& start,
                            const estl::vector::Vector<double, 3>& dir,
                            IntersectData& inter) {
  Vector<double, 3> local_start(mat_inv_ * Vector<double, 4>(start, 1), 0),
      dist;
  double a = dot(normal_, dir);
  double b = dot(normal_, local_start) - constant_;
  if (a == 0) {
    return false;
  } else if (b / a < 0) {
    return false;
  }
  inter.t_near = b / a;
  inter.mat = material_;
  inter.point = start + (dir * inter.t_near);
  inter.normal = normal_;
  dist = inter.point - origin_;
  if (length(dist) > radius_) {
    return false;
  }
  return true;
}

std::unique_ptr<ray::Object> ray::GenerateSphere(double radius, Material mat) {
  return std::unique_ptr<Object>(new Sphere(radius, mat));
}
std::unique_ptr<ray::Object> ray::GeneratePlane(
    estl::vector::Vector<double, 3> origin,
    estl::vector::Vector<double, 3> normal, Material mat) {
  return std::unique_ptr<Object>(new Plane(origin * -1, normal, mat));
}
std::unique_ptr<ray::Object> ray::GenerateCircle(
    estl::vector::Vector<double, 3> origin,
    estl::vector::Vector<double, 3> normal, double radius, Material mat) {
  return std::unique_ptr<Object>(new Circle(origin, normal, radius, mat));
}

// bool ray::Object::IntersectPlane(const estl::vector::Vector<double, 3>&
// start,
//     const estl::vector::Vector<double, 3>& dir, double& t){
//   double a = dot(center, dir);
//   double b = dot(center, start) - plane_const;
//   if(a == 0){
//     return false;
//   }else if(b / a < 0){
//     return false;
//   }
//   t = b / a;
//   return true;
// }

// ray::Object::Object(std::vector<std::string> str,
//                     std::map<std::string, Material> mats) {
//   mat.fill_diagonal(1);
//   mat_inv.fill_diagonal(1);
//   std::map<std::string, Material>::iterator mat;
//   for (std::string& it : str) {
//     if (it.size() == 0) {
//       continue;
//     }
//     if (it[0] == 'o') {
//       name = it.substr(2);
//     } else if (it[0] == 'v' && it[1] == ' ') {
//       double x = 0, y = 0, z = 0;
//       sscanf(it.c_str(), "v %lf %lf %lf", &x, &y, &z);
//       vertices.push_back(x);
//       vertices.push_back(y);
//       vertices.push_back(z);
//       center[0] += x;
//       center[1] += y;
//       center[2] += z;
//       value_max[0] = std::max(value_max[0], x);
//       value_max[1] = std::max(value_max[1], y);
//       value_max[2] = std::max(value_max[2], z);
//     } else if (it[0] == 'v' && it[1] == 'n') {
//       double x = 0, y = 0, z = 0;
//       sscanf(it.c_str(), "vn %lf %lf %lf", &x, &y, &z);
//       vertex_normals.push_back(x);
//       vertex_normals.push_back(y);
//       vertex_normals.push_back(z);
//     } else if (it[0] == 'f' && it[1] == ' ') {
//       char a[20], b[20], c[20];
//       std::vector<std::string> tokens;
//       sscanf(it.c_str(), "f %s %s %s", a, b, c);
//       std::array<std::array<int, 3>, 3> face;
//       tokens = split(std::string(a), '/');
//       for (auto& it : tokens) {
//         if (it == std::string()) {
//           it = "0";
//         }
//       }
//       face[0][0] = std::stoi(tokens[0]);
//       face[1][0] = std::stoi(tokens[1]);
//       face[2][0] = std::stoi(tokens[2]);
//       tokens = split(std::string(b), '/');
//       for (auto& it : tokens) {
//         if (it == std::string()) {
//           it = "0";
//         }
//       }
//       face[0][1] = std::stoi(tokens[0]);
//       face[1][1] = std::stoi(tokens[1]);
//       face[2][1] = std::stoi(tokens[2]);
//       tokens = split(std::string(c), '/');
//       for (auto& it : tokens) {
//         if (it == std::string()) {
//           it = "0";
//         }
//       }
//       face[0][2] = std::stoi(tokens[0]);
//       face[1][2] = std::stoi(tokens[1]);
//       face[2][2] = std::stoi(tokens[2]);
//       faces[mat->second].push_back(face);
//     } else if (it.compare(0, 6, "usemtl") == 0) {
//       mat = mats.find(it.substr(7));
//       if (mat == mats.end()) {
//         std::cout << "Material not found!";
//       }
//     }
//   }
//   center[0] /= (vertices.size() / 3.0);
//   center[1] /= (vertices.size() / 3.0);
//   center[2] /= (vertices.size() / 3.0);
// }
//
// void ray::Object::SetVerticies(std::vector<double> verts) { vertices = verts;
// } void ray::Object::SetVerticies(std::vector<std::array<double, 3>> verts) {
//   vertices.clear();
//   for (auto& it : verts) {
//     vertices.push_back(it[0]);
//     vertices.push_back(it[1]);
//     vertices.push_back(it[2]);
//   }
// }
//
// void ray::Object::SetIndicies(ray::Material mat, std::vector<int> ints) {
//   for (std::size_t i = 0; i < ints.size(); i += 3) {
//     faces[mat].push_back(std::array<std::array<int, 3>, 3>{
//         {{{ints[i], ints[i + 1], ints[i + 2]}},
//          {{0, 0, 0}},
//          {{ints[i], ints[i + 1], ints[i + 2]}}}});
//   }
// }
//
// void ray::Object::SetIndicies(ray::Material mat,
//                               std::vector<std::array<int, 3>> ints) {
//   for (auto& it : ints) {
//     faces[mat].push_back(
//         std::array<std::array<int, 3>, 3>{{it, {{0, 0, 0}}, it}});
//   }
// }
//
// void ray::Object::CalculateNormals() {
//   std::vector<std::pair<int, Vector<double, 3>>> norms(vertices.size());
//   for (auto& mat : faces) {
//     for (auto& it : mat.second) {
//       Vector<double, 3> a({vertices[(3 * it[0][0])],
//                            vertices[(3 * it[0][0]) + 1],
//                            vertices[(3 * it[0][0]) + 2]});
//       Vector<double, 3> b({vertices[(3 * it[0][1])],
//                            vertices[(3 * it[0][1]) + 1],
//                            vertices[(3 * it[0][1]) + 2]});
//       Vector<double, 3> c({vertices[(3 * it[0][1])],
//                            vertices[(3 * it[0][1]) + 1],
//                            vertices[(3 * it[0][1]) + 2]});
//       norms[it[0][0]].first++;
//       norms[it[0][0]].second += cross(b - a, c - a);
//       norms[it[0][1]].first++;
//       norms[it[0][1]].second += cross(a - b, c - b);
//       norms[it[0][2]].first++;
//       norms[it[0][2]].second += cross(a - c, b - c);
//     }
//   }
//   for (auto& it : norms) {
//     it.second /= it.first;
//     vertex_normals.push_back(it.second[0]);
//     vertex_normals.push_back(it.second[1]);
//     vertex_normals.push_back(it.second[2]);
//   }
// }
//
// void ray::Object::Center() {
//   for (std::size_t i = 0; i < vertices.size(); i += 3) {
//     vertices[i] -= center[0];
//     vertices[i + 1] -= center[1];
//     vertices[i + 2] -= center[2];
//   }
//   value_max[0] -= center[0];
//   value_max[1] -= center[1];
//   value_max[2] -= center[2];
//   center = {{0, 0, 0}};
// }
//
// void ray::Object::Normalize(bool maintain_ratio) {
//   if (maintain_ratio == true) {
//     double factor =
//         std::max(value_max[0], std::max(value_max[1], value_max[2]));
//     for (auto& it : vertices) {
//       it /= factor;
//     }
//     value_max[0] /= factor;
//     value_max[1] /= factor;
//     value_max[2] /= factor;
//   } else {
//     for (std::size_t i = 0; i < vertices.size(); i += 3) {
//       vertices[i] /= value_max[0];
//       vertices[i + 1] /= value_max[1];
//       vertices[i + 2] /= value_max[2];
//     }
//     value_max = {{1, 1, 1}};
//   }
// }
//
// void ray::Object::Translate(double x, double y, double z) {
//   estl::matrix::Matrix<double, 4, 4> trans;
//   trans.fill_diagonal(1);
//   trans(0, 3) = x;
//   trans(1, 3) = y;
//   trans(2, 3) = z;
//   mat = trans * mat;
//   trans(0, 3) = -x;
//   trans(1, 3) = -y;
//   trans(2, 3) = -z;
//   mat_inv = mat_inv * trans;
// }
//
// bool ray::Object::IntersectSphere(const estl::vector::Vector<double, 3>&
// start,
//                                   const estl::vector::Vector<double, 3>& dir,
//                                   double& t) {
//   double t0, t1;
//   double a = dot(dir, dir);
//   double b = 2 * dot(dir, start);
//   double c = dot(start, start) -
//              std::max(value_max[0], std::max(value_max[1], value_max[2]));
//   if (Quadradic(a, b, c, t0, t1) == false) {
//     return false;
//   }
//   if (t0 > t1) {
//     std::swap(t0, t1);
//   }
//   if (t0 < 0) {
//     if (t1 < 0) {
//       return false;
//     }
//     t = t1;
//   } else {
//     t = t0;
//   }
//   return true;
// }
//
// bool ray::Object::IntersectPlane(const estl::vector::Vector<double, 3>&
// start,
//     const estl::vector::Vector<double, 3>& dir, double& t){
//   double a = dot(center, dir);
//   double b = dot(center, start) - plane_const;
//   if(a == 0){
//     return false;
//   }else if(b / a < 0){
//     return false;
//   }
//   t = b / a;
//   return true;
// }
//
// bool ray::Object::Intersect(const estl::vector::Vector<double, 3>& start,
//                             const estl::vector::Vector<double, 3>& dir,
//                             ray::Intersect& inter) {
//   Vector<double, 3> local_start(mat_inv * Vector<double, 4>(start, 1), 0);
//   if ((type == SPHERE || type == MESH) &&
//       IntersectSphere(local_start, dir, inter.t_near) == true) {
//     if (type == SPHERE) {
//       inter.mat = global_mat;
//       inter.point = start + (dir * inter.t_near);
//       inter.normal = normalize(local_start + (dir * inter.t_near));
//       return true;
//     }
//     return true;
//   } else if (type == PLANE &&
//              IntersectPlane(local_start, dir, inter.t_near) == true) {
//     inter.mat = global_mat;
//     inter.point = start + (dir * inter.t_near);
//     inter.normal = center;
//     return true;
//   } else {
//     return false;
//   }
// }
//
// bool ray::Object::IntersectPolygon(estl::vector::Vector<double, 3> start,
//                                    estl::vector::Vector<double, 3> vec,
//                                    double& t, Material& mat) {
//   return false;
// }
//
// std::vector<std::unique_ptr<ray::Object>> ray::ParseObjFile(
//     std::string file_path) {
//   std::ifstream file(file_path);
//   std::vector<std::unique_ptr<Object>> objects;
//   if (file.is_open()) {
//     std::string line;
//     bool reading = false;
//     std::vector<std::string> current;
//     std::map<std::string, Material> materials;
//     while (getline(file, line)) {
//       if (line.compare(0, 6, "mtllib") == 0) {
//         materials = ParseMaterialFile(
//             file_path.substr(0, file_path.rfind('/')) + "/" +
//             line.substr(7));
//       }
//       if (line.size() != 0 && line[0] == 'o') {
//         if (reading == true) {
//           objects.emplace_back(
//               std::make_unique<Object>(Object(current, materials)));
//           objects.back()->Center();
//           current.clear();
//         } else {
//           reading = true;
//         }
//       }
//       if (reading == true) {
//         current.push_back(line.substr(0, line.find('#')));
//       }
//     }
//     objects.emplace_back(std::make_unique<Object>(Object(current,
//     materials))); objects.back()->Center(); file.close();
//   }
//   return objects;
// }
//
// std::unique_ptr<ray::Object> ray::GenerateSphere(double radius, Material mat)
// {
//   std::unique_ptr<Object> obj = std::make_unique<ray::Object>(Object());
//   obj->value_max[0] = radius;
//   obj->type = Object::SPHERE;
//   obj->global_mat = mat;
//   return obj;
// }
//
// std::unique_ptr<ray::Object> ray::GeneratePlane(
//     estl::vector::Vector<double, 3> center,
//     estl::vector::Vector<double, 3> normal, Material mat) {
//   std::unique_ptr<Object> obj = std::make_unique<ray::Object>(Object());
//   obj->plane_const = -dot(center, normal);
//   obj->center = normal;
//   obj->type = Object::PLANE;
//   obj->global_mat = mat;
//   return obj;
// }
