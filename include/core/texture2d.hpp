#pragma once

#include "handle.hpp"
#include "utils/image.hpp"

namespace ptah {

class Texture2D {
 private:
  Texture2DHandle m_handle;
  unsigned int m_ToGlFormat(ImageFormat format);
  void m_SetTextureParams();

 public:
  explicit Texture2D(Image& image);
  void Bind(unsigned int slot);
};

}  // namespace ptah