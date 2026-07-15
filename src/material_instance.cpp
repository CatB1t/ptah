#include "material_instance.hpp"

#include "material.hpp"

namespace ptah {

MaterialInstance::MaterialInstance(Material& base)
    : m_base(base), m_block(base.Size()) {
  m_block.SetData(m_base.m_default_block.data(), m_base.m_default_block.size(),
                  0);
}

void MaterialInstance::Bind() {
  m_block.Sync();
  m_block.GPUBuffer().BindUniform(1);
}

Material& MaterialInstance::Base() { return m_base; }

}  // namespace ptah