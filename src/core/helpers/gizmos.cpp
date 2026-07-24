#include "core/helpers/gizmos.hpp"

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/renderer.hpp"

namespace ptah {
Gizmos::Gizmos()
    : m_quadmesh(m_MakeQuad()),
      m_gizmo_material(PTAH_SHADERS_DIR "/default.vert",
                       PTAH_SHADERS_DIR "/gizmo.frag"),
      m_light_gizmo(utils::load_image(PTAH_GIZMOS_DIR "/point_light.png")),
      m_light_texture{m_light_gizmo.value()},
      m_grid_material(PTAH_SHADERS_DIR "/default.vert",
                      PTAH_SHADERS_DIR "/grid.frag") {
  m_grid_instance = m_grid_material.createInstance();
  m_light_instance = m_gizmo_material.createInstance();
  m_light_instance->SetTexture(&m_light_texture, ptah::TextureSlot::Albedo);
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
  renderer.m_SetState(m_gizmo_material.Props());
  renderer.m_Draw(draw_cmd, m_gizmo_material.Props());
}

void Gizmos::DrawGrid(Renderer& renderer) {
  glm::mat4 model{1.0};
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::scale(model, glm::vec3(50.0));
  m_grid_material.Use();
  m_grid_material.Set("uModel", model);
  m_grid_material.Set("uModelInverse", glm::inverse(glm::mat3(model)));
  renderer.m_SetState(m_grid_material.Props());
  renderer.m_Draw(m_quadmesh.GetDrawCommand(model, *m_grid_instance),
                  m_grid_material.Props());
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

}  // namespace ptah
