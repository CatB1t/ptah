#pragma once

#include "core/texture.hpp"
#include "core/texture_props.hpp"
#include "handle.hpp"
#include "utils/image.hpp"

namespace ptah {

class Texture2D : public Texture {
 private:
  Texture2DHandle m_handle;
  unsigned int m_ToGlFormat(ImageFormat format);
  void m_SetTextureParams(TextureProps props);

 public:
  explicit Texture2D(Image& image, TextureProps props = {});
  void Bind(unsigned int slot) override;
  Texture2DHandle Handle();
};

}  // namespace ptah