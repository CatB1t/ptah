#pragma once

#include <glm/vec4.hpp>
#include <vector>

#include "camera.hpp"
#include "data_buffer.hpp"
#include "draw_command.hpp"
#include "material.hpp"
#include "material_instance.hpp"
#include "material_props.hpp"
#include "utils/file_loading.hpp"

namespace ptah {

struct RendererSettings {
  glm::vec4 background{0.1, 0.1, 0.1, 1.0};
  Material default_material{
      ptah::utils::load_file(PTAH_SHADERS_DIR "/default.vert"),
      ptah::utils::load_file(PTAH_SHADERS_DIR "/default.frag")};
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

  void m_Draw(const DrawCommand& cmd, MaterialProps& props);
  MaterialInstance* m_ResolveMaterial(MaterialInstance* other);

 public:
  Renderer(unsigned int width, unsigned int height);
  ~Renderer();
  void Begin(const Camera& camera, float time = 0.0f);
  void Submit(const DrawCommand& cmd);
  void Flush();
};
}  // namespace ptah