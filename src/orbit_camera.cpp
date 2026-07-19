#include "orbit_camera.hpp"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "input.hpp"
#include "input_keys.hpp"
#include "utils/logger.hpp"

namespace ptah {

OrbitCamera::OrbitCamera() {}

void OrbitCamera::m_Rotate(const glm::vec2& mouse_delta) {
  if (glm::length(mouse_delta) <= 0.001) return;
  m_azimuth += mouse_delta.x * m_vertical_speed;
  m_altitude -= mouse_delta.y * m_horizontal_speed;
  m_azimuth = std::fmod(m_azimuth, 2.0f * kPI);
  m_altitude = std::clamp(m_altitude, -1.0f, 1.0f);
}

void OrbitCamera::Update(Input& input) {
  if (input.IsMouseDown(MouseButton::Left)) {
    auto delta = input.MouseDelta();
    m_Rotate(delta);
  }

  auto scroll_delta = input.MouseScroll();
  m_distance += scroll_delta.y;
  m_distance = std::max(m_distance, 1.0f);
}

Camera OrbitCamera::Data() {
  float radius = glm::sqrt(1.01f - m_altitude * m_altitude);
  m_position =
      glm::normalize(glm::vec3(radius * glm::cos(m_azimuth), m_altitude,
                               radius * glm::sin(m_azimuth))) *
      m_distance;
  glm::mat4 view = glm::lookAt(m_position, m_target, glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection =
      glm::perspective(glm::radians(60.0), 16.0 / 9.0, 0.01, 500.0);
  return Camera{view, projection};
}

}  // namespace ptah