#pragma once

#include "core/draw_command.hpp"
#include "core/material.hpp"
#include "core/mesh.hpp"
#include "core/texture_cube.hpp"

namespace ptah {
class MaterialInstance;

class Environment {

 private:
  Mesh m_unitcube;
  TextureCube m_skybox;
  Material m_material;
  MaterialInstance* m_instance;

 public:
  Environment();
  DrawCommand GetDrawCommand();
};

}  // namespace ptah
