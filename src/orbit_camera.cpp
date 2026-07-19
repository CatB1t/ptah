#include "orbit_camera.hpp"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "input.hpp"
#include "input_keys.hpp"
#include "utils/logger.hpp"

namespace ptah {

OrbitCamera::OrbitCamera(Window& window, double fov, double z_near,
                         double z_far)
    : m_fov(fov), m_near(z_near), m_far(z_far) {
  auto size = window.Size();
  if (size.y != 0) {
    m_aspect_ratio = size.x / static_cast<double>(size.y);
  }
  window.AddResizeCallback([&](unsigned int width, unsigned int height) {
    if (height == 0) return;
    m_Resize(width, height);
  });
}

void OrbitCamera::m_Resize(double width, double height) {
  m_aspect_ratio = width / height;
}

glm::vec3 OrbitCamera::m_CameraPos() {
  float radius = glm::sqrt(1.01f - m_altitude * m_altitude);
  auto cameraPos = glm::normalize(glm::vec3(
      radius * glm::cos(m_azimuth), m_altitude, radius * glm::sin(m_azimuth)));
  return cameraPos * m_distance + m_target;
}

void OrbitCamera::Update(Input& input) {
  // Rotate
  if (input.IsMouseDown(MouseButton::Left)) {
    auto mouse_delta = input.MouseDeltaNormalized();
    if (glm::length(mouse_delta) > 0.0f) {
      m_azimuth += mouse_delta.x * m_horizontal_speed;
      m_altitude -= mouse_delta.y * m_vertical_speed;
      m_azimuth = std::fmod(m_azimuth, 2.0f * kPI);
      m_altitude = std::clamp(m_altitude, -1.0f, 1.0f);
    }
  }

  // Pan
  if (input.IsMouseDown(MouseButton::Right)) {
    auto mouse_delta = input.MouseDeltaNormalized();
    if (glm::length(mouse_delta) > 0.0f) {
      glm::vec3 cameraForward{glm::normalize(m_target - m_CameraPos())};
      glm::vec3 cameraUp{0.0, 1.0, 0.0};
      glm::vec3 cameraRight =
          glm::normalize(glm::cross(cameraForward, cameraUp));
      cameraUp = glm::cross(cameraForward, cameraRight);
      m_target -= cameraRight * mouse_delta.x * m_pan_speed;
      m_target += cameraUp * mouse_delta.y * m_pan_speed;
    }
  }

  // Zoom
  auto scroll_delta = input.MouseScroll();
  if (glm::length(scroll_delta) > 0.0f) {
    m_distance += scroll_delta.y * m_zoom_speed;
    m_distance = std::max(m_distance, 1.0f);
  }
}

Camera OrbitCamera::Data() {
  glm::mat4 view =
      glm::lookAt(m_CameraPos(), m_target, glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection =
      glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
  return Camera{view, projection};
}

}  // namespace ptah