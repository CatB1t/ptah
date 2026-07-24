#pragma once

#include <optional>

#include "core/light.hpp"
#include "core/material.hpp"
#include "core/mesh.hpp"
#include "core/texture2d.hpp"
#include "utils/image.hpp"

namespace ptah {
class Renderer;
class Gizmos {
 private:
  Mesh m_quadmesh;
  Material m_gizmo_material;
  Material m_grid_material;
  std::optional<Image> m_light_gizmo;
  Texture2D m_light_texture;
  MaterialInstance* m_grid_instance;
  MaterialInstance* m_light_instance;

  Mesh m_MakeQuad();

 public:
  Gizmos();
  void DrawPointLight(Renderer& renderer, const PointLight& point_light,
                      const glm::vec3& view_position);
  void DrawGrid(Renderer& renderer);
};
}  // namespace ptah