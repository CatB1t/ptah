#include "input.hpp"

#include <GLFW/glfw3.h>

#include "core/window.hpp"

namespace ptah {

Input::Input() {}

Input::Input(Window& window) { Attach(window); }

void Input::Attach(Window& window) {
  window.AddKeyCallback([&](int key, int action, int scancode) {
    m_keys[key] = (action != GLFW_RELEASE);
  });

  window.AddMouseCallback([&](int key, int action, int mods) {
    m_mouse_keys[key] = (action != GLFW_RELEASE);
  });

  window.AddMousePositionCallback(
      [&](double x, double y) { m_mousepos = {x, -y}; });

  window.AddMouseScrollCallback(
      [&](double x, double y) { m_mousescroll = {x, -y}; });
}

void Input::Update() {
  m_keys_prev = m_keys;
  m_mouse_keys_prev = m_mouse_keys;
  m_mousepos_prev = m_mousepos;
  m_mousescroll = {0, 0};
}

bool Input::IsDown(int key) const { return m_keys[key]; }
bool Input::IsPressed(int key) const {
  return m_keys[key] && !m_keys_prev[key];
}
bool Input::IsReleased(int key) const {
  return !m_keys[key] && m_keys_prev[key];
}

bool Input::IsMouseDown(MouseButton key) const {
  return m_mouse_keys[static_cast<int>(key)];
}
bool Input::IsMousePressed(MouseButton key) const {
  return m_mouse_keys[static_cast<int>(key)] &&
         !m_mouse_keys_prev[static_cast<int>(key)];
}
bool Input::IsMouseReleased(MouseButton key) const {
  return !m_mouse_keys[static_cast<int>(key)] &&
         m_mouse_keys_prev[static_cast<int>(key)];
}

glm::vec2 Input::MouseDelta() const { return m_mousepos - m_mousepos_prev; }

glm::vec2 Input::MousePosition() const { return m_mousepos; }

glm::vec2 Input::MouseScroll() const { return m_mousescroll; }

}  // namespace ptah
