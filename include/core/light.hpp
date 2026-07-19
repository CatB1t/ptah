#pragma once

#include <glm/vec3.hpp>

#include "core/constants.hpp"

namespace ptah {

struct DirectionalLight {
  glm::vec3 color{constants::colors::WHITE};
  glm::vec3 direction{1.0f, 0.0f, 0.0f};
  float intensity = 0.2f;
};

struct PointLight {
  glm::vec3 color{constants::colors::WHITE};
  glm::vec3 position{0.0f};
  float intensity = 1.0f;
};

}  // namespace ptah
