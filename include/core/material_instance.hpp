#pragma once

#include <array>

#include "core/material.hpp"
#include "core/mirrored_buffer.hpp"
#include "core/texture2d.hpp"
#include "core/texture_slot.hpp"

namespace ptah {

class MaterialInstance {
 private:
  Material& m_base;
  MirroredBuffer m_block;
  std::array<Texture2D*, (unsigned int)TextureSlot::Count> m_textures{};

 public:
  MaterialInstance(Material& base);
  void SetTexture(Texture2D* texture, TextureSlot slot);
  void Bind();
  Material& Base();

  template <typename T>
  void SetBlockUniform(const char* name, const T& data) {
    if (!m_base.m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN("Material block does not contain {}, value is ignored.",
                       name);
      return;
    }

    Layout& layout = m_base.m_block_uniforms.at(name);
    PTAH_RENDER_DEBUG("{} at {} updated ", name, layout.offset);
    m_block.SetData(static_cast<const void*>(&data), sizeof(data),
                    layout.offset);
  };
};

};  // namespace ptah