#include "core/helpers/gizmos.hpp"

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/constants.hpp"
#include "core/material_props.hpp"
#include "core/renderer.hpp"

namespace ptah {
Gizmos::Gizmos()
    : m_quadmesh(m_MakeQuad()),
      m_axeslines(m_MakeAxes()),
      m_line(m_MakeLine()),
      m_gizmo_material(PTAH_SHADERS_DIR "/default.vert",
                       PTAH_SHADERS_DIR "/gizmo.frag"),
      m_grid_material(PTAH_SHADERS_DIR "/default.vert",
                      PTAH_SHADERS_DIR "/grid.frag"),
      m_axes_material(PTAH_SHADERS_DIR "/default.vert",
                      PTAH_SHADERS_DIR "/axes.frag"),
      m_line_material(PTAH_SHADERS_DIR "/default.vert",
                      PTAH_SHADERS_DIR "/axes.frag"),
      m_light_gizmo(
          utils::load_image(PTAH_ENGINE_ASSETS_DIR "/gizmos/point_light.png")),
      m_light_texture{m_light_gizmo.value()} {
  m_grid_material.props.cull = true;
  m_grid_instance = m_grid_material.createInstance();
  m_light_instance = m_gizmo_material.createInstance();
  m_light_instance->SetTexture(&m_light_texture, ptah::TextureSlot::Albedo);
  m_axes_material.props.draw_mode = DrawMode::Lines;
  m_axes_material.props.cull = false;
  m_axes_material.props.depth_test = false;
  m_axes_instance = m_axes_material.createInstance();

  m_line_material.props.draw_mode = DrawMode::Lines;
  m_line_material.props.cull = false;
  m_line_instance = m_line_material.createInstance();

  m_dir_light_instance = m_gizmo_material.createInstance();
  auto sun_icon =
      utils::load_image(PTAH_ENGINE_ASSETS_DIR "/gizmos/dir_light.png");
  m_dir_light_texture = new Texture2D(sun_icon.value());
  m_dir_light_instance->SetTexture(m_dir_light_texture,
                                   ptah::TextureSlot::Albedo);
}

void Gizmos::DrawDirLight(Renderer& renderer, const DirectionalLight& light,
                          const glm::vec3& view_position) {
  auto material = m_dir_light_instance->Base();
  float radius = 10.0f;
  glm::vec3 dis = view_position - light.direction;
  glm::mat4 model{1.0f};
  model = glm::translate(model, light.direction * radius);
  model = glm::rotate(model, glm::atan(dis.x, dis.z), glm::vec3(0.0, 1.0, 0.0));
  material.Use();
  material.Set("uModel", model);
  material.Set("uModelInverse", glm::inverse(glm::mat3(model)));
  m_dir_light_instance->SetBlockUniform("color", glm::vec4(light.color, 1.0));
  m_dir_light_instance->Bind();
  auto draw_cmd = m_quadmesh.GetDrawCommand(model, *m_dir_light_instance);
  renderer.m_SetState(m_gizmo_material.props);
  renderer.m_Draw(draw_cmd, m_gizmo_material.props);

  glm::mat4 line_model{1.0f};
  line_model = glm::scale(line_model, light.direction * radius);
  m_line_material.Use();
  m_line_material.Set("uModel", line_model);
  m_line_material.Set("uModelInverse", glm::mat3(1.0f));
  renderer.m_SetState(m_line_material.props);
  renderer.m_Draw(m_line.GetDrawCommand(glm::mat4{1.0}, *m_line_instance),
                  m_line_material.props);
}

void Gizmos::DrawPointLight(Renderer& renderer, const PointLight& point_light,
                            const glm::vec3& view_position) {
  glm::vec3 dis = view_position - point_light.position;
  glm::mat4 model{1.0f};
  model = glm::translate(model, point_light.position);
  model = glm::rotate(model, glm::atan(dis.x, dis.z), glm::vec3(0.0, 1.0, 0.0));

  auto material = m_light_instance->Base();
  material.Use();
  material.Set("uModel", model);
  material.Set("uModelInverse", glm::inverse(glm::mat3(model)));
  m_light_instance->SetBlockUniform("color", glm::vec4(point_light.color, 1.0));
  m_light_instance->Bind();
  auto draw_cmd = m_quadmesh.GetDrawCommand(model, *m_light_instance);
  renderer.m_SetState(m_gizmo_material.props);
  renderer.m_Draw(draw_cmd, m_gizmo_material.props);
}

void Gizmos::DrawGrid(Renderer& renderer) {
  glm::mat4 model{1.0};
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::scale(model, glm::vec3(50.0));
  m_grid_material.Use();
  m_grid_material.Set("uModel", model);
  m_grid_material.Set("uModelInverse", glm::inverse(glm::mat3(model)));
  renderer.m_SetState(m_grid_material.props);
  renderer.m_Draw(m_quadmesh.GetDrawCommand(model, *m_grid_instance),
                  m_grid_material.props);
}

void Gizmos::DrawAxes(Renderer& renderer) {
  m_axes_material.Use();
  m_axes_material.Set("uModel", glm::mat4(1.0f));
  m_axes_material.Set("uModelInverse", glm::mat3(1.0f));
  renderer.m_SetState(m_axes_material.props);
  renderer.m_Draw(m_axeslines.GetDrawCommand(glm::mat4{1.0}, *m_axes_instance),
                  m_axes_material.props);
}

Mesh Gizmos::m_MakeQuad() {
  const glm::vec3 normal{0.0f, 0.0f, 1.0f};
  const glm::vec3 tangent{1.0f, 0.0f, 0.0f};

  std::vector<Vertex> vertices{
      {{-0.5f, -0.5f, 0.0f}, normal, {0.0f, 0.0f}, tangent},  // bottom-left
      {{0.5f, -0.5f, 0.0f}, normal, {1.0f, 0.0f}, tangent},   // bottom-right
      {{0.5f, 0.5f, 0.0f}, normal, {1.0f, 1.0f}, tangent},    // top-right
      {{-0.5f, 0.5f, 0.0f}, normal, {0.0f, 1.0f}, tangent},   // top-left
  };

  std::vector<unsigned int> indices{0, 1, 2, 2, 3, 0};

  return Mesh(vertices, indices);
}

Mesh Gizmos::m_MakeAxes() {
  // We use the normal as a color attribute here, duplicated vertices are needed
  // but it's trivial.
  std::vector<Vertex> vertices{
      {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},
      {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},

      {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},
      {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},

      {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},
      {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},
  };

  std::vector<unsigned int> indices{0, 1, 2, 3, 4, 5};

  return Mesh(vertices, indices);
}

Mesh Gizmos::m_MakeLine() {
  std::vector<Vertex> vertices{
      {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},
      {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, glm::vec3{0.0f}},
  };

  return Mesh(vertices);
}

}  // namespace ptah
