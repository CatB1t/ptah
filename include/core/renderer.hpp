#pragma once

#include <glm/vec4.hpp>
#include <vector>

#include "core/camera.hpp"
#include "core/constants.hpp"
#include "core/data_buffer.hpp"
#include "core/draw_command.hpp"
#include "core/material.hpp"
#include "core/material_instance.hpp"
#include "core/material_props.hpp"
#include "core/mesh.hpp"
#include "core/shader_defines.hpp"
#include "core/window.hpp"
#include "light.hpp"
#include "materials/materials_factory.hpp"
#include "utils/file_loading.hpp"

namespace ptah {

struct RendererSettings {
  glm::vec4 background{constants::colors::BLACK};
  Material default_material = MakeLambert();
  MaterialInstance* default_instance;
  bool override_materials = false;
  bool override_instances = false;
};

class Renderer {
 private:
  struct alignas(16) _PointLight {
    glm::vec4 position{0.0f};
    glm::vec4 color = constants::colors::WHITE;
  };

  struct alignas(16) PerFrameData {
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 vp;
    glm::vec4 view_position;
    glm::vec4 dir_light_color;
    glm::vec4 dir_light_dir;
    _PointLight point_lights[PTAH_N_POINT_LIGHTS];
    int n_active_point_lights;
    float time;
  };

  unsigned int m_width;
  unsigned int m_height;
  RendererSettings m_settings;
  std::vector<DrawCommand> m_commands;
  DataBuffer m_frame_data;
  PerFrameData m_per_frame_data;
  Mesh m_quadmesh;
  Material m_gizmo_material;
  std::optional<Image> m_light_gizmo;
  Texture2D m_light_texture;
  Material m_grid_material;
  MaterialInstance* m_grid_instance;

  // Lights
  DirectionalLight default_light{};
  std::vector<PointLight> m_pointlights;
  std::array<MaterialInstance*, PTAH_N_POINT_LIGHTS> m_plights_material_pool{};

  void m_SetGrid();
  void m_SetPointLights();
  void m_UploadPerFrameData();
  void m_SetState(MaterialProps& props);
  void m_Draw(const DrawCommand& cmd, MaterialProps& props);
  MaterialInstance* m_ResolveMaterial(MaterialInstance* other);
  Mesh m_MakeQuad();

 public:
  Renderer(Window& window);
  ~Renderer();
  void Begin(const Camera& camera, float time = 0.0f);
  void Submit(const DrawCommand& command);
  void Submit(const std::vector<DrawCommand>& commands);
  void Submit(const PointLight& light);
  void Submit(const std::vector<PointLight>& point_lights);
  void Flush();
  void Resize(unsigned int width, unsigned int height);
  Material& defaultMaterial();
};
}  // namespace ptah