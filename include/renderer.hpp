#pragma once

#include <vector>

#include "draw_command.h"

namespace ptah {
class Renderer {
 private:
  unsigned int m_width;
  unsigned int m_height;
  std::vector<DrawCommand> m_commands;

 public:
  Renderer(unsigned int width, unsigned int height);
  ~Renderer();
  void Submit(const DrawCommand& cmd);
  void Flush();
};
}  // namespace ptah