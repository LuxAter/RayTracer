#ifndef RAY_OBJECT_HPP_
#define RAY_OBJECT_HPP_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <estl/basic/matrix.hpp>
#include <estl/basic/vector.hpp>

#include "material.hpp"
#include "math.hpp"

namespace ray {

struct IntersectData;

class Object {
 public:
  enum Type { DIFFUSE, REFLECTION, REFLECTION_REFRACTION };
  Object();
  virtual ~Object();

  void Translate(double x, double y, double z);

  virtual bool Intersect(const estl::base::Vec3d& start,
                         const estl::base::Vec3d& dir,
                         IntersectData& inter);

  std::string name;
  Type type = DIFFUSE;

 protected:
  estl::base::Mat4d mat_, mat_inv_;
};

class Sphere : public Object {
 public:
  Sphere(const double& radius, Material mat = Material());

  bool Intersect(const estl::base::Vec3d& start,
                 const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double radius_, radius_square_;
};

class Plane : public Object {
 public:
  Plane(const estl::base::Vec3d& origin,
        const estl::base::Vec3d& normal,
        Material mat = Material());

  bool Intersect(const estl::base::Vec3d& start,
                 const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double constant_;
  estl::base::Vec3d origin_, normal_;
};

class Circle : public Object {
 public:
  Circle(const estl::base::Vec3d& origin,
         const estl::base::Vec3d& normal, const double& radius,
         Material mat = Material());
  bool Intersect(const estl::base::Vec3d& start,
                 const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double constant_, radius_;
  estl::base::Vec3d origin_, normal_;
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
//   bool IntersectSphere(const estl::base::Vec3d& start,
//                        const estl::base::Vec3d& vec, double&
//                        t);
//   bool IntersectPlane(const estl::base::Vec3d& start,
//                       const estl::base::Vec3d& vec, double& t);
//   bool IntersectPolygon(estl::base::Vec3d start,
//                         estl::base::Vec3d vec, double& t,
//                         Material& mat);
//   bool Intersect(const estl::base::Vec3d& start,
//                  const estl::base::Vec3d& dir, Intersect&
//                  inter);
//
//   Type type = SPHERE;
//   std::string name;
//   double plane_const = 0;
//   std::vector<double> vertices;
//   std::vector<double> vertex_normals;
//   estl::base::Vec3d center = {{0, 0, 0}};
//   std::array<double, 3> value_max = {{0, 0, 0}};
//   std::map<Material, std::vector<std::array<std::array<int, 3>, 3>>> faces;
//   Material global_mat;
//   estl::base::Mat4d mat, mat_inv;
//
//  private:
//   std::array<std::array<int, 3>, 3> ParseFace(std::string str);
// };

struct IntersectData {
  double t_near = INFINITY;
  Material mat;
  estl::base::Vec3d normal;
  estl::base::Vec3d point;
  const Object* obj;
};

std::vector<std::unique_ptr<Object>> ParseObjFile(std::string file_path);
std::unique_ptr<Object> GenerateSphere(double radius,
                                       Material mat = Material());
std::unique_ptr<Object> GeneratePlane(estl::base::Vec3d origin,
                                      estl::base::Vec3d normal,
                                      Material mat = Material());
std::unique_ptr<Object> GenerateCircle(estl::base::Vec3d origin,
                                       estl::base::Vec3d normal,
                                       double radius,
                                       Material mat = Material());
}  // namespace ray

#endif  // RAY_OBJECT_HPP_
