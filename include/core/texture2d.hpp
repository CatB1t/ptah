#pragma once

#include "core/texture2d_props.hpp"
#include "handle.hpp"
#include "utils/image.hpp"

namespace ptah {

class Texture2D {
 private:
  Texture2DHandle m_handle;
  unsigned int m_ToGlFormat(ImageFormat format);
  void m_SetTextureParams(Texture2D_Props props);

 public:
  explicit Texture2D(Image& image, Texture2D_Props props = {});
  void Bind(unsigned int slot);
};

}  // namespace ptah