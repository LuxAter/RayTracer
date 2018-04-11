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

struct IntersectData;

class Object {
 public:
  Object();
  virtual ~Object();

  void Translate(double x, double y, double z);

  virtual bool Intersect(const estl::vector::Vector<double, 3>& start,
                         const estl::vector::Vector<double, 3>& dir,
                         IntersectData& inter);

 protected:
  std::string name;
  estl::matrix::Matrix<double, 4, 4> mat_, mat_inv_;
};

class Sphere : public Object {
  Sphere(const double& radius, Material mat=Material());

  bool Intersect(const estl::vector::Vector<double, 3>& start,
                 const estl::vector::Vector<double, 3>& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double radius_, radius_square_;
};
// class Object {
//  public:
//   enum Type { MESH, SPHERE, PLANE };
//   Object();
//   Object(std::vector<std::string> str, std::map<std::string, Material> mats);
//
//   void SetVerticies(std::vector<double> verts);
//   void SetVerticies(std::vector<std::array<double, 3>> verts);
//   void SetIndicies(Material, std::vector<int> ints);
//   void SetIndicies(Material, std::vector<std::array<int, 3>> ints);
//   void CalculateNormals();
//
//   void Center();
//   void Normalize(bool maintain_ratio = true);
//
//   void Translate(double x, double y, double z);
//
//   bool IntersectSphere(const estl::vector::Vector<double, 3>& start,
//                        const estl::vector::Vector<double, 3>& vec, double&
//                        t);
//   bool IntersectPlane(const estl::vector::Vector<double, 3>& start,
//                       const estl::vector::Vector<double, 3>& vec, double& t);
//   bool IntersectPolygon(estl::vector::Vector<double, 3> start,
//                         estl::vector::Vector<double, 3> vec, double& t,
//                         Material& mat);
//   bool Intersect(const estl::vector::Vector<double, 3>& start,
//                  const estl::vector::Vector<double, 3>& dir, Intersect&
//                  inter);
//
//   Type type = SPHERE;
//   std::string name;
//   double plane_const = 0;
//   std::vector<double> vertices;
//   std::vector<double> vertex_normals;
//   estl::vector::Vector<double, 3> center = {{0, 0, 0}};
//   std::array<double, 3> value_max = {{0, 0, 0}};
//   std::map<Material, std::vector<std::array<std::array<int, 3>, 3>>> faces;
//   Material global_mat;
//   estl::matrix::Matrix<double, 4, 4> mat, mat_inv;
//
//  private:
//   std::array<std::array<int, 3>, 3> ParseFace(std::string str);
// };

struct IntersectData {
  double t_near = INFINITY;
  Material mat;
  estl::vector::Vector<double, 3> normal;
  estl::vector::Vector<double, 3> point;
  const Object* obj;
};

std::vector<std::unique_ptr<Object>> ParseObjFile(std::string file_path);
std::unique_ptr<Object> GenerateSphere(double radus, Material mat);
std::unique_ptr<Object> GeneratePlane(estl::vector::Vector<double, 3> center,
                                      estl::vector::Vector<double, 3> normal,
                                      Material mat);
}  // namespace ray

#endif  // RAY_OBJECT_HPP_
