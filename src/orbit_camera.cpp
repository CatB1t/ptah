#include "orbit_camera.hpp"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "core/constants.hpp"
#include "input.hpp"
#include "input_keys.hpp"
#include "utils/logger.hpp"

namespace ptah {

OrbitCamera::OrbitCamera(Window& window, double fov, double z_near,
                         double z_far)
    : m_fov(fov), m_near(z_near), m_far(z_far) {
  window.AddResizeCallback([&](unsigned int width, unsigned int height) {
    if (height == 0) return;
    m_Resize(width, height);
  });

  auto size = window.Size();
  m_Resize(size.x, size.y);
}

void OrbitCamera::m_Resize(double width, double height) {
  if (height == 0) return;
  m_aspect_ratio = width / height;
  m_projection =
      glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
}

glm::vec3 OrbitCamera::m_CameraPos() {
  if (!m_pos_dirty) return m_position;
  float radius = glm::sqrt(1.0f - m_altitude * m_altitude);
  radius = glm::clamp(radius, 0.01f, 0.99f);
  auto cameraPos = glm::normalize(glm::vec3(
      radius * glm::cos(m_azimuth), m_altitude, radius * glm::sin(m_azimuth)));
  m_position = cameraPos * m_distance + m_target;
  m_pos_dirty = false;
  return m_position;
}

void OrbitCamera::Update(Input& input) {
  // Rotate
  if (input.IsMouseDown(MouseButton::Left)) {
    auto mouse_delta = input.MouseDeltaNormalized();
    if (glm::length(mouse_delta) > 0.0f) {
      m_azimuth += mouse_delta.x * m_horizontal_speed;
      m_altitude -= mouse_delta.y * m_vertical_speed;
      m_azimuth = std::fmod(m_azimuth, 2.0f * constants::PI);
      m_altitude = std::clamp(m_altitude, -1.0f, 1.0f);
      m_pos_dirty = true;
    }
  }

  // Pan
  if (input.IsMouseDown(MouseButton::Right)) {
    auto mouse_delta = input.MouseDeltaNormalized();
    if (glm::length(mouse_delta) > 0.0f) {
      glm::vec3 cameraForward{glm::normalize(m_target - m_CameraPos())};
      glm::vec3 cameraRight =
          glm::normalize(glm::cross(cameraForward, constants::UP));
      glm::vec3 cameraUp = glm::cross(cameraForward, cameraRight);
      m_target -= cameraRight * mouse_delta.x * m_pan_speed;
      m_target += cameraUp * mouse_delta.y * m_pan_speed;
      m_pos_dirty = true;
    }
  }

  // Zoom
  auto scroll_delta = input.MouseScroll();
  if (glm::length(scroll_delta) > 0.0f) {
    m_distance += scroll_delta.y * m_zoom_speed;
    m_distance = std::max(m_distance, 1.0f);
    m_pos_dirty = true;
  }
}

Camera OrbitCamera::Data() {
  glm::mat4 view = glm::lookAt(m_CameraPos(), m_target, constants::UP);
  return Camera{view, m_projection};
}

}  // namespace ptah