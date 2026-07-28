#include "core/material_instance.hpp"

#include <utility>

#include "core/material.hpp"

namespace ptah {

MaterialInstance::MaterialInstance(Material& base)
    : m_base(base), m_block(base.Size()) {
  m_block.SetData(m_base.m_default_block.data(), m_base.m_default_block.size(),
                  0);
}

void MaterialInstance::SetTexture(Texture* texture, TextureSlot slot) {
  // TODO: Handle current texture disposal
  m_textures[std::to_underlying(slot)] = texture;
}

Texture* MaterialInstance::GetTexture(TextureSlot slot, bool fall_to_base) {
  int slot_idx = std::to_underlying(slot);
  auto texture = m_textures[slot_idx];
  if (fall_to_base) {
    return texture == nullptr ? m_base.m_ResolveTexture(slot) : texture;
  }
  return texture;
}

void MaterialInstance::Bind() {
  m_block.Sync();
  m_block.GPUBuffer().BindUniform(1);
  for (int slot = 0; slot < m_textures.size(); slot++) {
    Texture* texture = m_textures[slot];
    if (texture == nullptr) {
      texture = m_base.m_ResolveTexture(static_cast<TextureSlot>(slot));
    }
    texture->Bind(slot);
  }
}

Material& MaterialInstance::Base() { return m_base; }

}  // namespace ptah