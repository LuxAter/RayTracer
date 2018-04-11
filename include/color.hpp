#ifndef RAY_COLOR_HPP_
#define RAY_COLOR_HPP_

#include <iostream>

#include <estl/vector.hpp>

namespace ray {
  class Color{
    public:
      Color();
      Color(int red, int green, int blue);
      Color(double red, double green, double blue);
      explicit Color(estl::vector::Vector<double, 3> vec);

      Color& operator+=(const Color& rhs);

      double r, g, b;
  };

  Color Combine(Color lhs, double lhs_perc, Color rhs, double rhs_perc);

  Color operator+(Color lhs, Color rhs);
  Color operator-(Color lhs, Color rhs);
  Color operator*(Color lhs, Color rhs);
  Color operator/(Color lhs, Color rhs);
  Color operator+(Color lhs, double rhs);
  Color operator-(Color lhs, double rhs);
  Color operator*(Color lhs, double rhs);
  Color operator/(Color lhs, double rhs);
  Color operator+(double lhs, Color rhs);
  Color operator-(double lhs, Color rhs);
  Color operator*(double lhs, Color rhs);
  Color operator/(double lhs, Color rhs);

  Color Pow(Color val, double exp);
  Color Normalize(Color val);

  std::ostream& operator<<(std::ostream& out, Color col);
} // namespace ray
#endif  // RAY_COLOR_HPP_
