#include "core/renderer.hpp"

#include <glad/gl.h>

#include <algorithm>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/data_buffer.hpp"
#include "core/helpers/gizmos.hpp"
#include "core/material.hpp"
#include "core/material_props.hpp"
#include "core/shader_defines.hpp"
#include "core/window.hpp"
#include "utils/logger.hpp"

namespace ptah {

Renderer::Renderer(Window& window)
    : m_width(window.Size().x),
      m_height(window.Size().y),
      settings{},
      m_frame_data(BufferType::UNIFORM, sizeof(PerFrameData)) {
  Material::InitDefaults();
  glViewport(0, 0, m_width, m_height);
  glEnable(GL_MULTISAMPLE);

  settings.default_material.SetBlockUniform("color",
                                            glm::vec4(0.0, 0.0, 0.0, 1.0));
  settings.default_instance = settings.default_material.createInstance();
  settings.default_instance->SetBlockUniform("color",
                                             glm::vec4(1.0, 0.0, 0.0, 1.0));

  window.AddResizeCallback(
      [&](unsigned int width, unsigned int height) { Resize(width, height); });
}

Renderer::~Renderer() { Material::DestroyDefaults(); }

void Renderer::Begin(const Camera& camera, float time) {
  m_per_frame_data.view = camera.view;
  m_per_frame_data.projection = camera.projection;
  m_per_frame_data.vp = camera.projection * camera.view,
  m_per_frame_data.time = time;
  glm::vec3 camera_position = glm::vec3(glm::inverse(m_per_frame_data.view)[3]);
  m_per_frame_data.view_position = glm::vec4(camera_position, 1.0);

  m_per_frame_data.dir_light_color =
      glm::vec4(m_dirlight.color, m_dirlight.intensity);
  m_per_frame_data.dir_light_dir =
      glm::vec4(glm::normalize(m_dirlight.direction), 1.0f);
}

void Renderer::Submit(const DrawCommand& command) {
  m_commands.push_back(command);
}

void Renderer::Submit(const std::vector<DrawCommand>& commands) {
  m_commands.insert(m_commands.end(), commands.begin(), commands.end());
}

void Renderer::Submit(const DirectionalLight& light) { m_dirlight = light; }

void Renderer::Submit(const PointLight& light) {
  if (m_pointlights.size() == PTAH_N_POINT_LIGHTS) {
    PTAH_RENDER_WARN("Maximum lights reached, discarding.");
    return;
  }
  m_pointlights.push_back(light);
}

void Renderer::Submit(const std::vector<PointLight>& point_lights) {
  if (m_pointlights.size() + point_lights.size() > PTAH_N_POINT_LIGHTS) {
    int slots_available = PTAH_N_POINT_LIGHTS - m_pointlights.size();
    PTAH_RENDER_WARN("Maximum lights reached, inserting only {}.",
                     slots_available);
    m_pointlights.insert(m_pointlights.end(), point_lights.begin(),
                         point_lights.begin() + slots_available);
    return;
  }

  m_pointlights.insert(m_pointlights.end(), point_lights.begin(),
                       point_lights.end());
}

MaterialInstance* Renderer::m_ResolveMaterial(MaterialInstance* other) {
  return (settings.override_materials) ? settings.default_instance : other;
}

void Renderer::m_DrawEnvironment() {
  if (!settings.draw_skybox) return;
  auto cmd = m_environment.GetDrawCommand();
  Material& material = cmd.material->Base();
  material.Use();
  material.Set("uModel", cmd.transform);
  cmd.material->Bind();
  m_SetState(material.props);
  m_Draw(cmd, material.props);
}

void Renderer::m_SetPointLights() {
  m_per_frame_data.n_active_point_lights = m_pointlights.size();
  for (int i = 0; i < m_pointlights.size(); i++) {
    auto& pl = m_pointlights[i];
    m_per_frame_data.point_lights[i] = {glm::vec4(pl.position, 1.0),
                                        glm::vec4(pl.color, pl.intensity)};
  }
}

void Renderer::m_UploadPerFrameData() {
  m_frame_data.SetData(&m_per_frame_data, sizeof(m_per_frame_data));
  m_frame_data.BindUniform(0);
}

void Renderer::m_SetState(MaterialProps& props) {
  if (props.cull) {
    glEnable(GL_CULL_FACE);
    glCullFace(props.CullingFace());
  } else {
    glDisable(GL_CULL_FACE);
  }

  if (props.depth_test) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  if (props.depth_write) {
    glDepthMask(GL_TRUE);
  } else {
    glDepthMask(GL_FALSE);
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
    glDrawElements(props.DrawingMode(), cmd.count, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(props.DrawingMode(), 0, cmd.count);
  }
}

void Renderer::Flush() {
  m_SetPointLights();
  m_UploadPerFrameData();

  glClearColor(settings.background.r, settings.background.g,
               settings.background.b, settings.background.a);

  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
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
      material.Use();
      m_SetState(material.props);
      last_material = &material;
    };

    if (settings.override_materials && settings.override_instances) {
      material_instance->Bind();
    } else {
      cmd.material->Bind();
    }

    material.Set("uModel", cmd.transform);
    material.Set("uModelInverse", glm::inverse(glm::mat3(cmd.transform)));
    m_Draw(cmd, material.props);
  }

  if (settings.draw_light_gizmos) {
    for (int i = 0; i < m_pointlights.size(); i++) {
      m_gizmos.DrawPointLight(*this, m_pointlights[i],
                              glm::vec3(m_per_frame_data.view_position));
    }

    m_gizmos.DrawDirLight(*this, m_dirlight,
                          glm::vec3(m_per_frame_data.view_position));
  }

  if (settings.draw_grid) m_gizmos.DrawGrid(*this);
  if (settings.draw_axes) m_gizmos.DrawAxes(*this);

  m_DrawEnvironment();
  m_commands.clear();
  m_pointlights.clear();
}

void Renderer::Resize(unsigned int width, unsigned int height) {
  glViewport(0, 0, width, height);
}

Material& Renderer::defaultMaterial() { return settings.default_material; };

}  // namespace ptah
