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
  struct alignas(16) PerFrameData {
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 vp;
    glm::vec4 dir_light_color;
    glm::vec4 dir_light_dir;
    float time;
  };

  unsigned int m_width;
  unsigned int m_height;
  RendererSettings m_settings;
  std::vector<DrawCommand> m_commands;
  DataBuffer m_frame_data;

  // Lights
  DirectionalLight default_light{};

  void m_SetState(MaterialProps& props);
  void m_Draw(const DrawCommand& cmd, MaterialProps& props);
  MaterialInstance* m_ResolveMaterial(MaterialInstance* other);

 public:
  Renderer(Window& window);
  ~Renderer();
  void Begin(const Camera& camera, float time = 0.0f);
  void Submit(const DrawCommand& command);
  void Submit(const std::vector<DrawCommand>& commands);
  void Flush();
  void Resize(unsigned int width, unsigned int height);
  Material& defaultMaterial();
};
}  // namespace ptah