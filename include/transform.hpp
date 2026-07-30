#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace ptah {
struct Transform {
  glm::vec3 position{0.0};
  glm::vec3 rotation{0.0};
  glm::vec3 scale{1.0};

  glm::mat4 GetMatrix() const {
    glm::mat4 m{1.0};
    m = glm::translate(m, position);
    m = glm::rotate(m, glm::radians(rotation.x), glm::vec3{1.0, 0.0, 0.0});
    m = glm::rotate(m, glm::radians(rotation.y), glm::vec3{0.0, 1.0, 0.0});
    m = glm::rotate(m, glm::radians(rotation.z), glm::vec3{0.0, 0.0, 1.0});
    m = glm::scale(m, scale);
    return m;
  }

  void Reset() {
    position = glm::vec3{0.0};
    rotation = glm::vec3{0.0};
    scale = glm::vec3{1.0};
  }
};
}  // namespace ptah
