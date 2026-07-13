#pragma once

#include <string>

#include "handle.hpp"

namespace ptah {
class Shader {
 private:
  Handle m_program;

  unsigned int m_LoadShaderSource(const std::string& source, unsigned int type);
  void m_CheckCompileStatus(unsigned int id, const char* type);
  void m_CheckLinkStatus(const Handle& program);

 public:
  Shader(const std::string& vertex_source, const std::string& fragment_source);
  void Use();
  void Dispose();
};
}  // namespace ptah