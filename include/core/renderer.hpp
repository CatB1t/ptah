#pragma once

#include <glm/vec4.hpp>
#include <vector>

#include "core/camera.hpp"
#include "core/data_buffer.hpp"
#include "core/draw_command.hpp"
#include "core/material.hpp"
#include "core/material_instance.hpp"
#include "core/material_props.hpp"
#include "core/window.hpp"
#include "utils/file_loading.hpp"

namespace ptah {

struct RendererSettings {
  glm::vec4 background{0.1, 0.1, 0.1, 1.0};
  Material default_material{PTAH_SHADERS_DIR "/default.vert",
                            PTAH_SHADERS_DIR "/default.frag"};
  MaterialInstance* default_instance;
  bool override_materials = false;
};

class Renderer {
 private:
  struct PerFrameData {
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 vp;
    float time;
  };

  unsigned int m_width;
  unsigned int m_height;
  RendererSettings m_settings;
  std::vector<DrawCommand> m_commands;
  DataBuffer m_frame_data;

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