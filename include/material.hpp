#pragma once

#include <glm/glm.hpp>
#include <string>

#include "handle.hpp"

namespace ptah {

struct Layout {
  std::string name;
  unsigned int type;  // GL Types
  int length;         // Number of elements
  int offset;         // Offset
};

class Material {
 private:
  MaterialHandle m_program;

  unsigned int m_LoadShaderSource(const std::string& source, unsigned int type);
  void m_CheckCompileStatus(unsigned int id, const char* type);
  void m_CheckLinkStatus(const MaterialHandle& program);
  int m_GetUniformLocation(const char* name);

 public:
  Material(const std::string& vertex_source,
           const std::string& fragment_source);
  void Set(const char* name, const glm::mat4& matrix);
  void Set(const char* name, const glm::vec4& vec);
  void Set(const char* name, const glm::vec3& vec);
  void Set(const char* name, float value);
  void Set(const char* name, int value);
  void Use();
  void ResolveLayout();
  void Dispose();
};
}  // namespace ptah