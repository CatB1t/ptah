#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "data_buffer.hpp"
#include "handle.hpp"
#include "utils/logger.hpp"

namespace ptah {

struct Layout {
  unsigned int type;  // GL Types
  int length;         // Number of elements
  int offset;         // Offset
};

class Material {
 private:
  MaterialHandle m_program;
  std::vector<uint8_t> m_block;
  std::unordered_map<std::string, Layout> m_block_uniforms;
  DataBuffer m_block_buffer;

  unsigned int m_LoadShaderSource(const std::string& source, unsigned int type);
  void m_CheckCompileStatus(unsigned int id, const char* type);
  void m_CheckLinkStatus(const MaterialHandle& program);
  int m_GetUniformLocation(const char* name);
  void m_ResolveLayout();

 public:
  Material(const std::string& vertex_source,
           const std::string& fragment_source);
  void Set(const char* name, const glm::mat4& matrix);
  void Set(const char* name, const glm::vec4& vec);
  void Set(const char* name, const glm::vec3& vec);
  void Set(const char* name, float value);
  void Set(const char* name, int value);
  void Use();
  void Dispose();

  template <typename T>
  void SetBlockUniform(const char* name, const T& data) {
    if (!m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN(
          "Material block does not contain {}, value is ignored. {}", name,
          m_block_uniforms.size());
      return;
    }

    Layout& layout = m_block_uniforms.at(name);
    assert(layout.offset + sizeof(data) <= m_block.size());
    memcpy(m_block.data() + layout.offset, &data, sizeof(data));
    PTAH_RENDER_DEBUG("{} at {} updated ", name, layout.offset);
    m_block_buffer.SetData(m_block.data(), m_block.size(), 0);
    m_block_buffer.BindUniform(1);
  };

};

}  // namespace ptah
