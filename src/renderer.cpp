#include "renderer.hpp"

#include <glad/gl.h>

#include "data_buffer.hpp"
#include "material_props.hpp"
#include "utils/logger.hpp"

namespace ptah {

Renderer::Renderer(unsigned int width, unsigned int height)
    : m_width(width),
      m_height(height),
      m_settings{},
      m_frame_data(BufferType::UNIFORM, sizeof(PerFrameData)) {
  glViewport(0, 0, m_width, m_height);

  m_settings.default_material.SetBlockUniform("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
  m_settings.default_instance = m_settings.default_material.createInstance();
  m_settings.default_instance->SetBlockUniform("color", glm::vec4(1.0, 0.0, 0.0, 1.0));
}

Renderer::~Renderer() {}

void Renderer::Begin(const Camera& camera, float time) {
  PerFrameData data{camera.projection, camera.view,
                    camera.projection * camera.view, time};

  m_frame_data.SetData(&data, sizeof(data));
  m_frame_data.BindUniform(0);
}

void Renderer::Submit(const DrawCommand& cmd) { m_commands.push_back(cmd); }

MaterialInstance* Renderer::m_ResolveMaterial(MaterialInstance* other) {
  return (m_settings.override_materials) ? m_settings.default_instance : other;
}

void Renderer::m_Draw(const DrawCommand& cmd, MaterialProps& props) {
  if (props.cull) {
    glEnable(GL_CULL_FACE);
    glCullFace(props.CullFace());
  } else {
    glDisable(GL_CULL_FACE);
  }

  if (props.depth_test) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  if (props.draw_mode == DrawMode::Wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  glBindVertexArray(cmd.handle.Id());
  if (cmd.indexed) {
    glDrawElements(props.DrawMode(), cmd.count, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(props.DrawMode(), 0, cmd.count);
  }
}

void Renderer::Flush() {
  glClearColor(m_settings.background.r, m_settings.background.g,
               m_settings.background.b, m_settings.background.a);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  MaterialInstance* material_instance = m_ResolveMaterial(nullptr);
  for (auto cmd : m_commands) {
    Material& material = material_instance->Base();
    material.Use();
    material.Set("uModel", cmd.transform);
    material_instance->Bind();
    m_Draw(cmd, material.Props());
  }
  m_commands.clear();
}

}  // namespace ptah