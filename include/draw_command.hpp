#pragma once

#include "glm/mat4x4.hpp"
#include "handle.hpp"

namespace ptah {

struct DrawCommand {
  glm::mat4 transform;
  MeshHandle handle;
  unsigned int count;
  bool indexed;
};

}  // namespace ptah
