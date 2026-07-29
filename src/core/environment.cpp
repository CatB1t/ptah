#include "core/environment.hpp"

#include "core/helpers/primitives.hpp"
#include "core/material_instance.hpp"

namespace ptah {

Environment::Environment()
    : m_unitcube{primitives::MakeCube()},
      m_skybox{TextureCube(PTAH_ENGINE_ASSETS_DIR "/skybox/")},
      m_material{PTAH_SHADERS_DIR "/skybox.vert",
                 PTAH_SHADERS_DIR "/skybox.frag"} {
  m_material.props.depth_write = false;
  m_instance = m_material.createInstance();
  m_instance->SetTexture(&m_skybox, TextureSlot::Environment);
}

void Environment::BindSkybox(Material& material) {
  material.SetTexture(TextureSlot::Environment, &m_skybox);
}

DrawCommand Environment::GetDrawCommand() {
  return m_unitcube.GetDrawCommand(glm::mat4{1.0}, *m_instance);
}

void Environment::SetSkybox(TextureCube skybox) {
  // m_skybox.Dispose();
  m_skybox = skybox;
}

}  // namespace ptah
