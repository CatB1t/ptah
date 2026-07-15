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