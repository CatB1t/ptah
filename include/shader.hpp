#pragma once

#include <string>

#include "handle.hpp"

namespace ptah {
class Shader {
 private:
  Handle m_program;

 public:
  Shader(const std::string& vertex_source, const std::string& fragment_source);
  void Use();
  void Dispose();
};
}  // namespace ptah