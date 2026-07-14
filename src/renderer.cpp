#include "renderer.hpp"

#include <glad/gl.h>

#include "data_buffer.hpp"
#include "utils/logger.hpp"

namespace ptah {

Renderer::Renderer(unsigned int width, unsigned int height)
    : m_width(width),
      m_height(height),
      m_settings{},
      m_frame_data(BufferType::UNIFORM, sizeof(PerFrameData)) {
  glViewport(0, 0, m_width, m_height);
}

Renderer::~Renderer() {}

void Renderer::Begin(const Camera& camera, float time) {
  PerFrameData data{camera.projection, camera.view,
                    camera.projection * camera.view, time};

  m_frame_data.SetData(&data, sizeof(data));
  m_frame_data.BindUniform(0);
}

void Renderer::Submit(const DrawCommand& cmd) { m_commands.push_back(cmd); }

Shader* Renderer::m_ResolveShader(Shader* other) {
  return (m_settings.override_shaders) ? &m_settings.default_shader : other;
}

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
  Shader* effective_shader = m_ResolveShader(nullptr);
  for (auto cmd : m_commands) {
    effective_shader->Use();
    effective_shader->Set("uModel", cmd.transform);
    m_Draw(cmd);
  }
  m_commands.clear();
}

}  // namespace ptah