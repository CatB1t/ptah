#pragma once

#include <vector>

namespace ptah {

enum class ImageFormat : unsigned int { RGB = 0, RGBA = 1 };

struct Image {
  int width;
  int height;
  std::vector<unsigned char> data;
  ImageFormat format;
};

}  // namespace ptah