#pragma once

#include "core/camera.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace ptah {

class Input;

class OrbitCamera {
 private:
  const float kPI = glm::pi<float>();
  glm::vec3 m_position{0.0, 0.0, -1.0};
  glm::vec3 m_target{0.0};
  float m_altitude = 0.1f;
  float m_azimuth = 0.0f;
  float m_distance = 50.0f;

  const float m_horizontal_speed = 0.005;
  const float m_vertical_speed = 0.01;

  void m_Rotate(const glm::vec2& mouse_delta);

 public:
  OrbitCamera();
  void Update(Input& input);
  Camera Data();
};

}  // namespace ptah