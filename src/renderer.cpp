#include "renderer.hpp"

#include <glad/gl.h>

namespace ptah {

Renderer::Renderer(unsigned int width, unsigned int height)
    : m_width(width), m_height(height), m_settings{} {
  glViewport(0, 0, m_width, m_height);
}

Renderer::~Renderer() {}

void Renderer::Begin(const Camera& camera) {
  // TODO: use uniform buffer
  m_settings.default_shader.Set("uProjection", camera.projection);
  m_settings.default_shader.Set("uView", camera.view);
  m_settings.default_shader.Set("uProjView", camera.projection * camera.view);
}

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
  glClearColor(m_settings.background.r, m_settings.background.g,
               m_settings.background.b, m_settings.background.a);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (m_settings.override_shaders) {
    m_settings.default_shader.Use();
  }
  Shader* effective_shader = &m_settings.default_shader;
  for (auto cmd : m_commands) {
    effective_shader->Set("uModel", cmd.transform);
    m_Draw(cmd);
  }
  m_commands.clear();
}

}  // namespace ptah