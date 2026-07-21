#include "core/material_instance.hpp"

#include "core/material.hpp"

namespace ptah {

MaterialInstance::MaterialInstance(Material& base)
    : m_base(base), m_block(base.Size()) {
  m_block.SetData(m_base.m_default_block.data(), m_base.m_default_block.size(),
                  0);
}

void MaterialInstance::SetTexture(Texture2D* texture, TextureSlot slot) {
  m_textures[(unsigned int)slot] = texture;
}

void MaterialInstance::Bind() {
  m_block.Sync();
  m_block.GPUBuffer().BindUniform(1);
  for (int slot = 0; slot < m_textures.size(); slot++) {
    Texture2D* texture = m_textures[slot];
    if (texture == nullptr) {
      texture = m_base.m_ResolveTexture(static_cast<TextureSlot>(slot));
    }
    texture->Bind(slot);
  }
}

Material& MaterialInstance::Base() { return m_base; }

}  // namespace ptah