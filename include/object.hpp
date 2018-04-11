#ifndef RAY_OBJECT_HPP_
#define RAY_OBJECT_HPP_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <estl/matrix.hpp>
#include <estl/vector.hpp>

#include "material.hpp"

namespace ray {

struct Intersect;

class Object {
 public:
  Object();
  Object(std::vector<std::string> str, std::map<std::string, Material> mats);

  void SetVerticies(std::vector<double> verts);
  void SetVerticies(std::vector<std::array<double, 3>> verts);
  void SetIndicies(Material, std::vector<int> ints);
  void SetIndicies(Material, std::vector<std::array<int, 3>> ints);
  void CalculateNormals();

  void Center();
  void Normalize(bool maintain_ratio = true);

  void Translate(double x, double y, double z);

  bool IntersectSphere(const estl::vector::Vector<double, 3>& start,
                       const estl::vector::Vector<double, 3>& vec, double& t);
  bool IntersectPolygon(estl::vector::Vector<double, 3> start,
                        estl::vector::Vector<double, 3> vec, double& t,
                        Material& mat);
  bool Intersect(const estl::vector::Vector<double, 3>& start,
                 const estl::vector::Vector<double, 3>& dir, Intersect& inter);

  bool sphere = false;
  std::string name;
  std::vector<double> vertices;
  std::vector<double> vertex_normals;
  estl::vector::Vector<double, 3> center = {{0, 0, 0}};
  std::array<double, 3> value_max = {{0, 0, 0}};
  std::map<Material, std::vector<std::array<std::array<int, 3>, 3>>> faces;
  Material sphere_mat;
  estl::matrix::Matrix<double, 4, 4> mat, mat_inv;

 private:
  std::array<std::array<int, 3>, 3> ParseFace(std::string str);
};

struct Intersect {
  double t_near;
  Material mat;
  estl::vector::Vector<double, 3> normal;
  estl::vector::Vector<double, 3> point;
  const Object* obj;
};

std::vector<std::unique_ptr<Object>> ParseObjFile(std::string file_path);
std::unique_ptr<Object> GenerateSphere(double radus, std::size_t faces);
}  // namespace ray

#endif  // RAY_OBJECT_HPP_
