#pragma once

#include <filesystem>

#include "core/handle.hpp"
#include "core/texture.hpp"
#include "core/texture_props.hpp"

namespace ptah {
class TextureCube : public Texture {
 private:
  TextureCubeHandle m_handle;

  void m_SetTextureParams(TextureProps props);

 public:
  TextureCube(const std::filesystem::path& textures_dir);
  void Bind(unsigned int slot) override;
};
}  // namespace ptah
