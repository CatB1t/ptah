#include "renderer.hpp"

#include <glad/gl.h>

namespace ptah {
Renderer::Renderer(unsigned int width, unsigned int height)
    : m_width(width), m_height(height) {}
Renderer::~Renderer() {}
void Renderer::Submit(const DrawCommand& cmd) { m_commands.push_back(cmd); }
void Renderer::Flush() {
  for (auto cmd : m_commands) {
    // Render
  }

  m_commands.clear();
}
}  // namespace ptah