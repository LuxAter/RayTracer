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

#include "basic/vector.hpp"

#include "material.hpp"
#include "math.hpp"

#include <iostream>

using namespace estl::base;

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
  mat_.Diagonal(1);
  mat_inv_.Diagonal(1);
}

ray::Object::~Object() {}

bool ray::Object::Intersect(const estl::base::Vec3d& start,
                            const estl::base::Vec3d& dir,
                            IntersectData& inter) {
  return false;
}

void ray::Object::Translate(double x, double y, double z) {
  estl::base::Mat4d trans;
  trans.Diagonal(1);
  trans(0, 3) = x;
  trans(1, 3) = y;
  trans(2, 3) = z;
  mat_ = Dot(trans, mat_);
  trans(0, 3) = -x;
  trans(1, 3) = -y;
  trans(2, 3) = -z;
  mat_inv_ = Dot(mat_inv_, trans);
}

ray::Sphere::Sphere(const double& radius, Material mat)
    : Object(),
      material_(mat),
      radius_(radius),
      radius_square_(radius * radius) {
  this->name = "sphere";
}

bool ray::Sphere::Intersect(const estl::base::Vec3d& start,
                            const estl::base::Vec3d& dir,
                            IntersectData& inter) {
  Vec3d local_start = Dot(mat_inv_, start);
  double t0, t1;
  double a = Dot(dir, dir);
  double b = 2 * Dot(dir, local_start);
  double c = Dot(local_start, local_start) - radius_square_;
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
  inter.normal = Normalize(local_start + (dir * inter.t_near));
  return true;
}

ray::Plane::Plane(const estl::base::Vec3d& origin,
                  const estl::base::Vec3d& normal, Material mat)
    : Object(),
      material_(mat),
      constant_(-Dot(origin, normal)),
      origin_(origin),
      normal_(Normalize(normal)) {
  this->name = "plane";
}
bool ray::Plane::Intersect(const estl::base::Vec3d& start,
                           const estl::base::Vec3d& dir, IntersectData& inter) {
  Vec3d local_start = Dot(mat_inv_, start);
  double a = Dot(normal_, dir);
  double b = constant_ - Dot(normal_, local_start);
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

ray::Circle::Circle(const estl::base::Vec3d& origin,
                    const estl::base::Vec3d& normal, const double& radius,
                    Material mat)
    : Object(),
      material_(mat),
      constant_(-Dot(origin, normal)),
      radius_(radius),
      origin_(origin),
      normal_(Normalize(normal)) {
  this->name = "plane";
}
bool ray::Circle::Intersect(const estl::base::Vec3d& start,
                            const estl::base::Vec3d& dir,
                            IntersectData& inter) {
  Vec3d local_start = Dot(mat_inv_, start), dist;
  double a = Dot(normal_, dir);
  double b = Dot(normal_, local_start) - constant_;
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
  if (Length(dist) > radius_) {
    return false;
  }
  return true;
}

ray::Triangle::Triangle(const estl::base::Vec3d& a, const estl::base::Vec3d& b,
                        const estl::base::Vec3d& c, Material mat)
    : Object(), material_(mat), a_(a), b_(b), c_(c) {
  this->name = "triangle";
}
bool ray::Triangle::Intersect(const estl::base::Vec3d& start,
                              const estl::base::Vec3d& dir,
                              IntersectData& inter) {
  Vec3d local_start = Dot(mat_inv_, start), dist;
  Vec3d ab = b_ - a_;
  Vec3d ac = c_ - a_;
  Vec3d p_vec = Cross(dir, ac);
  double det = Dot(ab, p_vec);
  if (fabs(det) < 1e-8) {
    return false;
  }
  double inv_det = 1.0 / det;
  Vec3d t_vec = local_start - a_;
  double u = Dot(t_vec, p_vec) * inv_det;
  if (u < 0 || u > 1) {
    return false;
  }
  Vec3d q_vec = Cross(t_vec, ab);
  double v = Dot(dir, q_vec) * inv_det;
  if (v < 0 || u + v > 1) {
    return false;
  }
  inter.t_near = Dot(ac, q_vec) * inv_det;
  inter.mat = material_;
  inter.point = start + (dir * inter.t_near);
  inter.normal = Normalize(Cross(ac, ab));
  return true;
}

ray::Mesh::Mesh(const std::vector<estl::base::Vec3d>& verticies,
                const std::vector<unsigned>& indicies, Material mat)
    : Object(), material_(mat), verticies_(verticies), indicies_(indicies) {
  this->name = "mesh";
  for (auto& it : verticies_) {
    radius_ = std::max(radius_, std::max(it.x, std::max(it.y, it.z)));
  }
  radius_square_ = radius_ * radius_;
}

bool ray::Mesh::Intersect(const estl::base::Vec3d& start,
                          const estl::base::Vec3d& dir, IntersectData& inter) {
  Vec3d local_start = Dot(mat_inv_, start);
  double t0, t1;
  double a = Dot(dir, dir);
  double b = 2 * Dot(dir, local_start);
  double c = Dot(local_start, local_start) - radius_square_;
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
  }
  bool intersect = false;
  for (unsigned i = 0; i < indicies_.size(); i += 3) {
    intersect = intersect ||
                IntersectTriangle(local_start, dir, verticies_[indicies_[i]],
                                  verticies_[indicies_[i + 1]],
                                  verticies_[indicies_[i + 2]], inter);
  }
  if (intersect) {
    inter.mat= material_;
    inter.point = start + (dir * inter.t_near);
  }
  return intersect;
}
bool ray::Mesh::IntersectTriangle(const estl::base::Vec3d& start,
                                  const estl::base::Vec3d& dir,
                                  const estl::base::Vec3d& a,
                                  const estl::base::Vec3d& b,
                                  const estl::base::Vec3d& c, IntersectData& inter) {
  Vec3d ab = c - a;
  Vec3d ac = b - a;
  Vec3d p_vec = Cross(dir, ac);
  double det = Dot(ab, p_vec);
  if ((det) < 1e-8) {
    return false;
  }
  double inv_det = 1.0 / det;
  Vec3d t_vec = start - a;
  double u = Dot(t_vec, p_vec) * inv_det;
  if (u < 0 || u > 1) {
    return false;
  }
  Vec3d q_vec = Cross(t_vec, ab);
  double v = Dot(dir, q_vec) * inv_det;
  if (v < 0 || u + v > 1) {
    return false;
  }
  double t = Dot(ac, q_vec) * inv_det;
  if (t >= inter.t_near) {
    return false;
  }
  inter.t_near = Dot(ac, q_vec) * inv_det;
  inter.normal = Normalize(Cross(ac, ab));
  return true;
}

std::unique_ptr<ray::Object> ray::GenerateSphere(double radius, Material mat) {
  return std::unique_ptr<Object>(new Sphere(radius, mat));
}
std::unique_ptr<ray::Object> ray::GeneratePlane(estl::base::Vec3d origin,
                                                estl::base::Vec3d normal,
                                                Material mat) {
  return std::unique_ptr<Object>(new Plane(origin * -1.0, normal, mat));
}
std::unique_ptr<ray::Object> ray::GenerateCircle(estl::base::Vec3d origin,
                                                 estl::base::Vec3d normal,
                                                 double radius, Material mat) {
  return std::unique_ptr<Object>(new Circle(origin, normal, radius, mat));
}
std::unique_ptr<ray::Object> ray::GenerateTriangle(estl::base::Vec3d a,
                                                   estl::base::Vec3d b,
                                                   estl::base::Vec3d c,
                                                   Material mat) {
  return std::unique_ptr<Object>(new Triangle(a, b, c, mat));
}

std::unique_ptr<ray::Object> ray::GenerateMesh(std::vector<estl::base::Vec3d> verticies,
                                     std::vector<unsigned> indicies,
                                     Material mat){
  return std::unique_ptr<Object>(new Mesh(verticies, indicies, mat));
}
