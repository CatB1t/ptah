#pragma once

#include <glm/mat4x4.hpp>

namespace ptah {

struct Camera {
  glm::mat4 view{1.0f};
  glm::mat4 projection{1.0f};
};

}  // namespace ptah
