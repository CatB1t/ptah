#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

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
  friend class MaterialInstance;
  MaterialHandle m_program;
  std::unordered_map<std::string, Layout> m_block_uniforms;
  int m_block_size = 0;
  std::vector<uint8_t> m_default_block;

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
  int Size();
  MaterialInstance* createInstance();

  template <typename T>
  void SetBlockUniform(const char* name, const T& data) {
    if (!m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN("Material block does not contain {}, value is ignored.",
                       name);
      return;
    }

    Layout& layout = m_block_uniforms.at(name);
    PTAH_RENDER_DEBUG("Material ({}): Default uniform {} updated ",
                      m_program.Id(), name);
    memcpy(m_default_block.data() + layout.offset, &data, sizeof(data));
  };
};

}  // namespace ptah
