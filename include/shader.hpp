#pragma once

#include <glm/glm.hpp>
#include <string>

#include "handle.hpp"

namespace ptah {
class Shader {
 private:
  ProgramHandle m_program;

  unsigned int m_LoadShaderSource(const std::string& source, unsigned int type);
  void m_CheckCompileStatus(unsigned int id, const char* type);
  void m_CheckLinkStatus(const ProgramHandle& program);
  int m_GetUniformLocation(const char* name);

 public:
  Shader(const std::string& vertex_source, const std::string& fragment_source);
  void Set(const char* name, const glm::mat4& matrix);
  void Set(const char* name, const glm::vec4& vec);
  void Set(const char* name, const glm::vec3& vec);
  void Set(const char* name, float value);
  void Set(const char* name, int value);
  void Use();
  void Dispose();
};
}  // namespace ptah