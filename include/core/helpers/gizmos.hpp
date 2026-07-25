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
  Mesh m_axeslines;
  Material m_gizmo_material;
  Material m_grid_material;
  Material m_axes_material;
  std::optional<Image> m_light_gizmo;
  Texture2D m_light_texture;
  Texture2D* m_dir_light_texture;
  MaterialInstance* m_grid_instance;
  MaterialInstance* m_light_instance;
  MaterialInstance* m_dir_light_instance;
  MaterialInstance* m_axes_instance;

  Mesh m_MakeQuad();
  Mesh m_MakeAxes();
  Mesh m_MakeLine();

 public:
  Gizmos();
  void DrawDirLight(Renderer& renderer, const DirectionalLight& light,
                    const glm::vec3& view_position);
  void DrawPointLight(Renderer& renderer, const PointLight& point_light,
                      const glm::vec3& view_position);
  void DrawGrid(Renderer& renderer);
  void DrawAxes(Renderer& renderer);
};
}  // namespace ptah