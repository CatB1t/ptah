#include "core/renderer.hpp"

#include <glad/gl.h>

#include <algorithm>

#include "core/data_buffer.hpp"
#include "core/material.hpp"
#include "core/material_props.hpp"
#include "core/window.hpp"
#include "utils/logger.hpp"

namespace ptah {

Renderer::Renderer(Window& window)
    : m_width(window.Size().x),
      m_height(window.Size().y),
      m_settings{},
      m_frame_data(BufferType::UNIFORM, sizeof(PerFrameData)) {
  Material::InitDefaults();
  glViewport(0, 0, m_width, m_height);

  m_settings.default_material.SetBlockUniform("color",
                                              glm::vec4(0.0, 0.0, 0.0, 1.0));
  m_settings.default_instance = m_settings.default_material.createInstance();
  m_settings.default_instance->SetBlockUniform("color",
                                               glm::vec4(1.0, 0.0, 0.0, 1.0));

  window.AddResizeCallback(
      [&](unsigned int width, unsigned int height) { Resize(width, height); });
}

Renderer::~Renderer() { Material::DestroyDefaults(); }

void Renderer::Begin(const Camera& camera, float time) {
  PerFrameData data{camera.view,
                    camera.projection,
                    camera.projection * camera.view,
                    glm::vec4(default_light.color, default_light.intensity),
                    glm::vec4(glm::normalize(default_light.direction), 1.0f),
                    time};

  m_frame_data.SetData(&data, sizeof(data));
  m_frame_data.BindUniform(0);
}

void Renderer::Submit(const DrawCommand& command) {
  m_commands.push_back(command);
}

void Renderer::Submit(const std::vector<DrawCommand>& commands) {
  m_commands.insert(m_commands.end(), commands.begin(), commands.end());
}

MaterialInstance* Renderer::m_ResolveMaterial(MaterialInstance* other) {
  return (m_settings.override_materials) ? m_settings.default_instance : other;
}

void Renderer::m_SetState(MaterialProps& props) {
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
}

void Renderer::m_Draw(const DrawCommand& cmd, MaterialProps& props) {
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

  std::vector<DrawCommand> sorted_commands{m_commands};
  std::sort(sorted_commands.begin(), sorted_commands.end(),
            [](DrawCommand& a, DrawCommand& b) {
              return a.material->Base() < b.material->Base();
            });

  Material* last_material = nullptr;

  for (auto cmd : sorted_commands) {
    auto material_instance = m_ResolveMaterial(cmd.material);
    Material& material = material_instance->Base();

    if (last_material != &material) {
      material.Reload();
      material.Use();
      m_SetState(material.Props());
      last_material = &material;
    };

    material_instance->Bind();
    material.Set("uModel", cmd.transform);
    m_Draw(cmd, material.Props());
  }
  m_commands.clear();
}

void Renderer::Resize(unsigned int width, unsigned int height) {
  glViewport(0, 0, width, height);
}

Material& Renderer::defaultMaterial() { return m_settings.default_material; };

}  // namespace ptah
