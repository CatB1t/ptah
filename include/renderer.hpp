#pragma once

#include <vector>

#include "draw_command.hpp"
#include "shader.hpp"

namespace ptah {
class Renderer {
 private:
  unsigned int m_width;
  unsigned int m_height;
  std::vector<DrawCommand> m_commands;
  Shader m_default_shader;

  void m_Draw(const DrawCommand& cmd);

 public:
  Renderer(unsigned int width, unsigned int height);
  ~Renderer();
  void Submit(const DrawCommand& cmd);
  void Flush();
};
}  // namespace ptah