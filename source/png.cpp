#include "png.hpp"

#include <png.h>

#include <iostream>

#include "math.hpp"

ray::Png::Png() {}

ray::Png::Png(std::string filename, unsigned width, unsigned height) {
  width_ = width;
  height_ = height;
  file_name_ = filename;
  pixel_data_ = (png_bytepp)malloc(height_ * sizeof(png_bytep));
  for (unsigned i = 0; i < height_; i++) {
    pixel_data_[i] = (png_bytep)malloc(6 * width_ * sizeof(png_byte));
  }
}

ray::Png::Png(const Png& copy) {
  width_ = copy.width_;
  height_ = copy.height_;
  file_name_ = copy.file_name_;
  pixel_data_ = (png_bytepp)malloc(height_ * sizeof(png_bytep));
  for (unsigned i = 0; i < height_; i++) {
    pixel_data_[i] = (png_bytep)malloc(6 * width_ * sizeof(png_byte));
  }
}

ray::Png::~Png() {
  for (unsigned i = 0; i < height_; ++i) {
    free(pixel_data_[i]);
  }
  free(pixel_data_);
}

ray::Png& ray::Png::operator=(const Png& copy){
  width_ = copy.width_;
  height_ = copy.height_;
  file_name_ = copy.file_name_;
  pixel_data_ = (png_bytepp)malloc(height_ * sizeof(png_bytep));
  for (unsigned i = 0; i < height_; i++) {
    pixel_data_[i] = (png_bytep)malloc(6 * width_ * sizeof(png_byte));
  }
  return *this;
}

void ray::Png::Plot(unsigned x, unsigned y, int r, int g, int b) {
  r = Clamp(r, 0, 65535);
  g = Clamp(g, 0, 65535);
  b = Clamp(b, 0, 65535);
  if (y < height_ && x < width_) {
    unsigned index = 6 * x;
    pixel_data_[y][index] = (char)std::floor(((double)r) / 256.0);
    pixel_data_[y][index + 1] = (char)(r % 256);
    pixel_data_[y][index + 2] =
        (char)std::floor(((double)g) / 256.0);
    pixel_data_[y][index + 3] = (char)(g % 256);
    pixel_data_[y][index + 4] =
        (char)std::floor(((double)b) / 256.0);
    pixel_data_[y][index + 5] = (char)(b % 256);
  }
}
void ray::Png::Plot(unsigned x, unsigned y, double r, double g, double b) {
  Plot(x, y, int(r * 65535 + 0.5), int(g * 65535 + 0.5), int(b * 65535 + 0.5));
}
void ray::Png::Fill(int r, int g, int b) {
  for (unsigned i = 0; i < height_; ++i) {
    for (unsigned j = 0; j < width_; ++j) {
      Plot(j, i, r, g, b);
    }
  }
}

void ray::Png::Fill(double r, double g, double b) {
  Fill(int(r * 65535 + 0.5), int(g * 65535 + 0.5), int(b * 65535 + 0.5));
}

bool ray::Png::Write() {
  FILE* file = fopen(file_name_.c_str(), "wb");
  if (!file) {
    std::cout << "Failed to open write file\n";
    return false;
  }
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    std::cout << "Failed to create png struct\n";
    return false;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    std::cout << "Failed to create png info struct\n";
    return false;
  }

  if (setjmp(png_jmpbuf(png))) {
    std::cout << "Error in libpng\n";
    return false;
  }

  png_init_io(png, file);

  png_set_IHDR(png, info, width_, height_, depth_, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);

  png_write_image(png, pixel_data_);
  png_write_end(png, NULL);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);
  fclose(file);

  return true;
}
