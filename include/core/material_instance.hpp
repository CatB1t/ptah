#pragma once

#include <array>

#include "core/material.hpp"
#include "core/mirrored_buffer.hpp"
#include "core/texture.hpp"
#include "core/texture_slot.hpp"

namespace ptah {

class MaterialInstance {
 private:
  Material& m_base;
  MirroredBuffer m_block;
  std::array<Texture*, (unsigned int)TextureSlot::Count> m_textures{};

 public:
  MaterialInstance(Material& base);
  void SetTexture(Texture* texture, TextureSlot slot);
  Texture* GetTexture(TextureSlot slot, bool fall_to_base = false);
  void Bind();
  Material& Base();

  template <typename T>
  T* View(const char* name) {
    if (!m_base.m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN("Requested uniform {} does not exist.", name);
      return nullptr;
    }

    Layout& layout = m_base.m_block_uniforms.at(name);
    return static_cast<T*>(m_block.At(layout.offset));
  }

  template <typename T>
  void SetBlockUniform(const char* name, const T& data) {
    if (!m_base.m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN("Material block does not contain {}, value is ignored.",
                       name);
      return;
    }

    Layout& layout = m_base.m_block_uniforms.at(name);
    m_block.SetData(static_cast<const void*>(&data), sizeof(data),
                    layout.offset);
  };
};

};  // namespace ptah