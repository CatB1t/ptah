#pragma once

#include <filesystem>

#include "core/handle.hpp"
#include "core/texture.hpp"
#include "core/texture2d_props.hpp"

namespace ptah {
class TextureCube : public Texture {
 private:
  TextureCubeHandle m_handle;

  void m_SetTextureParams(Texture2D_Props props);

 public:
  TextureCube(const std::filesystem::path& textures_dir);
  void Bind(unsigned int slot) override;
};
}  // namespace ptah
