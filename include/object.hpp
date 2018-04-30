#ifndef RAY_OBJECT_HPP_
#define RAY_OBJECT_HPP_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "basic/matrix.hpp"
#include "basic/vector.hpp"

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
                         const estl::base::Vec3d& dir, IntersectData& inter);

  std::string name;
  Type type = DIFFUSE;

 protected:
  estl::base::Mat4d mat_, mat_inv_;
};

class Sphere : public Object {
 public:
  Sphere(const double& radius, Material mat = Material());

  bool Intersect(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double radius_, radius_square_;
};

class Plane : public Object {
 public:
  Plane(const estl::base::Vec3d& origin, const estl::base::Vec3d& normal,
        Material mat = Material());

  bool Intersect(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double constant_;
  estl::base::Vec3d origin_, normal_;
};

class Circle : public Object {
 public:
  Circle(const estl::base::Vec3d& origin, const estl::base::Vec3d& normal,
         const double& radius, Material mat = Material());
  bool Intersect(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  double constant_, radius_;
  estl::base::Vec3d origin_, normal_;
};

class Triangle : public Object {
 public:
  Triangle(const estl::base::Vec3d& a, const estl::base::Vec3d& b,
           const estl::base::Vec3d& c, Material mat = Material());
  bool Intersect(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
                 IntersectData& inter);

 protected:
  Material material_;
  estl::base::Vec3d a_, b_, c_;
};

class Mesh : public Object {
 public:
  Mesh(const std::vector<estl::base::Vec3d>& verticies,
       const std::vector<unsigned>& indicies, Material mat = Material());
  bool Intersect(const estl::base::Vec3d& start, const estl::base::Vec3d& dir,
                 IntersectData& inter);
  bool IntersectTriangle(const estl::base::Vec3d& start,
                         const estl::base::Vec3d& dir,
                         const estl::base::Vec3d& a, const estl::base::Vec3d& b,
                         const estl::base::Vec3d& c, IntersectData& inter);

 protected:
  Material material_;
  std::vector<estl::base::Vec3d> verticies_;
  std::vector<unsigned> indicies_;
  double radius_, radius_square_;
};

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
                                       estl::base::Vec3d normal, double radius,
                                       Material mat = Material());
std::unique_ptr<Object> GenerateTriangle(estl::base::Vec3d a,
                                         estl::base::Vec3d b,
                                         estl::base::Vec3d c,
                                         Material mat = Material());
std::unique_ptr<Object> GenerateMesh(std::vector<estl::base::Vec3d> verticies,
                                     std::vector<unsigned> indicies,
                                     Material mat = Material());
}  // namespace ray

#endif  // RAY_OBJECT_HPP_
