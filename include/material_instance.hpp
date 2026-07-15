#pragma once

#include "data_buffer.hpp"
#include "material.hpp"

namespace ptah {
class MaterialInstance {
 private:
  Material& m_base;
  _SyncBuffer m_block;

 public:
  MaterialInstance(Material& base);
  void Bind();  // Called in loop, checks if data needs to be uploaded, and
                // binds the uniform to binding slot

  Material& Base();

  template <typename T>  // Called anywhere, set a uniform value
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