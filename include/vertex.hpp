#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace ptah {

struct Vertex {
  glm::vec3 position{0.0f};
  glm::vec3 normal{0.0f};
  glm::vec2 uv{0.0f};
  glm::vec3 tagnent{0.0f};
};

}  // namespace ptah