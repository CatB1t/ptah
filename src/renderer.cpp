#include "renderer.hpp"

#include <glad/gl.h>

#include "utils/file_loading.hpp"

namespace ptah {

Renderer::Renderer(unsigned int width, unsigned int height)
    : m_width(width),
      m_height(height),
      m_default_shader(
          ptah::utils::load_file(PTAH_SHADERS_DIR "/default.vert"),
          ptah::utils::load_file(PTAH_SHADERS_DIR "/default.frag")) {
  glViewport(0, 0, m_width, m_height);
}

Renderer::~Renderer() {}

void Renderer::Submit(const DrawCommand& cmd) { m_commands.push_back(cmd); }

void Renderer::m_Draw(const DrawCommand& cmd) {
  glBindVertexArray(cmd.handle.Id());
  if (cmd.indexed) {
    glDrawElements(GL_TRIANGLES, cmd.count, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, cmd.count);
  }
}

void Renderer::Flush() {
  glClearColor(0.1, 0.1, 0.1, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_default_shader.Use();
  for (auto cmd : m_commands) {
    m_Draw(cmd);
  }
  m_commands.clear();
}

}  // namespace ptah