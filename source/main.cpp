#include <iostream>

#include "material.hpp"

// using namespace estl::vector;

int main(int argc, char const* argv[])
{
  std::map<std::string, ray::Material> mats = ray::ParseMaterialFile("./flat_tree.mtl");
  for (auto &it: mats){
    std::cout << it.first << ": " << it.second << "\n";
  }
  /* code */
  return 0;
}
