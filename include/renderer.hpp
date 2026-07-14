#pragma once

#include <glm/vec4.hpp>
#include <vector>

#include "camera.hpp"
#include "draw_command.hpp"
#include "shader.hpp"
#include "utils/file_loading.hpp"

namespace ptah {

struct RendererSettings {
  glm::vec4 background{0.1, 0.1, 0.1, 1.0};
  Shader default_shader{
      ptah::utils::load_file(PTAH_SHADERS_DIR "/default.vert"),
      ptah::utils::load_file(PTAH_SHADERS_DIR "/default.frag")};
  bool override_shaders = true;
};

class Renderer {
 private:
  unsigned int m_width;
  unsigned int m_height;
  RendererSettings m_settings;
  std::vector<DrawCommand> m_commands;

  void m_Draw(const DrawCommand& cmd);
  Shader* m_ResolveShader(Shader* other) const;

 public:
  Renderer(unsigned int width, unsigned int height);
  ~Renderer();
  void Begin(const Camera& camera);
  void Submit(const DrawCommand& cmd);
  void Flush();
};
}  // namespace ptah