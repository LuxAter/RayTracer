#ifndef RAY_PNG_HPP_
#define RAY_PNG_HPP_

#include <string>

namespace ray {
  class Png{
    public:
      Png();
      ~Png();
    private:
      std::string file_name_, author_, text_description_, text_title_, text_software_;
  };
} // namespace ray
#endif  // RAY_PNG_HPP_
