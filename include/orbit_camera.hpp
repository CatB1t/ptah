#pragma once

#include "core/camera.hpp"
#include "core/window.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace ptah {

class Input;

class OrbitCamera {
 private:
  const float kPI = glm::pi<float>();
  glm::vec3 m_target{0.0, 0.0, 0.0};
  float m_altitude = 0.1f;
  float m_azimuth = 0.0f;
  float m_distance = 50.0f;

  const float m_horizontal_speed = 5.0f;
  const float m_vertical_speed = 2.0f;
  const float m_zoom_speed = 3.0f;
  const float m_pan_speed = 20.0f;

  double m_aspect_ratio = 1.0;
  double m_fov;
  double m_near;
  double m_far;

  glm::mat4 m_projection;

  void m_Resize(double width, double height);
  glm::vec3 m_CameraPos();

 public:
  OrbitCamera(Window& window, double fov = 45.0, double z_near = 0.01,
              double z_far = 500.0);
  void Update(Input& input);
  Camera Data();
};

}  // namespace ptah