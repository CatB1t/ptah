#include "material_instance.hpp"

#include "material.hpp"

namespace ptah {
MaterialInstance::MaterialInstance(Material& base)
    : m_base(base), m_block(base.Size()) {
      m_block.SetData(m_base.m_default_block.data(), m_base.m_default_block.size(), 0);
    }

// Called in loop, checks if data needs to be uploaded, and binds the uniform to
// binding slot
void MaterialInstance::Bind() {
  m_block.Sync();
  m_block.gpu_buffer.BindUniform(1);
}

Material& MaterialInstance::Base() { return m_base; }

}  // namespace ptah