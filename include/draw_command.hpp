#pragma once

#include "glm/mat4x4.hpp"
#include "handle.hpp"

namespace ptah {

class MaterialInstance;

struct DrawCommand {
  glm::mat4 transform;
  MeshHandle handle;
  MaterialInstance* material;  // TODO maybe use a handle later
  unsigned int count;
  bool indexed;
};

}  // namespace ptah
