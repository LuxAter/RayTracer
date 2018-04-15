#ifndef RAY_PNG_HPP_
#define RAY_PNG_HPP_

#include <string>
#include <vector>

#include "color.hpp"

namespace ray {
class Png {
 public:
  Png();
  Png(std::string filename, unsigned width, unsigned height);
  Png(const Png& copy);
  ~Png();

  Png& operator=(const Png& copy);

  void Plot(unsigned x, unsigned y, int r, int g, int b);
  void Plot(unsigned x, unsigned y, double r, double g, double b);
  void Fill(int r, int g, int b);
  void Fill(double r, double g, double b);

  bool Write();

 private:
  unsigned width_, height_, depth_ = 16;
  std::string file_name_;
  unsigned char** pixel_data_;
};
}  // namespace ray
#endif  // RAY_PNG_HPP_
