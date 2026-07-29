#include "input.hpp"

#include <GLFW/glfw3.h>

#include <utility>

#include "core/window.hpp"

namespace ptah {

Input::Input() {}

Input::Input(Window& window) { Attach(window); }

void Input::Attach(Window& window) {
  auto size = window.Size();
  m_width = size.x;
  m_height = size.y;

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

  window.AddResizeCallback([&](unsigned int width, unsigned int height) {
    if (height == 0) return;
    m_width = width;
    m_height = height;
  });
}

void Input::Update() {
  m_keys_prev = m_keys;
  m_mouse_keys_prev = m_mouse_keys;
  m_mousepos_prev = m_mousepos;
  m_mousescroll = {0, 0};
}

void Input::SetBlocked(bool blocked) {
  if (blocked == m_blocked) return;
  m_blocked = blocked;
  // Swallow the delta accumulated while blocked, otherwise the first unblocked
  // frame reports one big jump.
  if (!blocked) m_mousepos_prev = m_mousepos;
}

bool Input::IsBlocked() const { return m_blocked; }

bool Input::IsPressedRaw(KeyboardKey key) const {
  return m_keys[std::to_underlying(key)] &&
         !m_keys_prev[std::to_underlying(key)];
}

bool Input::IsDown(KeyboardKey key) const {
  return !m_blocked && m_keys[std::to_underlying(key)];
}
bool Input::IsPressed(KeyboardKey key) const {
  return !m_blocked && IsPressedRaw(key);
}
bool Input::IsReleased(KeyboardKey key) const {
  return !m_blocked && !m_keys[std::to_underlying(key)] &&
         m_keys_prev[std::to_underlying(key)];
}

bool Input::IsMouseDown(MouseButton key) const {
  return !m_blocked && m_mouse_keys[static_cast<int>(key)];
}
bool Input::IsMousePressed(MouseButton key) const {
  return !m_blocked && m_mouse_keys[static_cast<int>(key)] &&
         !m_mouse_keys_prev[static_cast<int>(key)];
}
bool Input::IsMouseReleased(MouseButton key) const {
  return !m_blocked && !m_mouse_keys[static_cast<int>(key)] &&
         m_mouse_keys_prev[static_cast<int>(key)];
}

glm::vec2 Input::MouseDelta() const {
  if (m_blocked) return {0.0f, 0.0f};
  return m_mousepos - m_mousepos_prev;
}

glm::vec2 Input::MouseDeltaNormalized() const {
  auto delta = MouseDelta();
  return {delta.x / (float)m_width, delta.y / (float)m_height};
}

glm::vec2 Input::MousePosition() const { return m_mousepos; }

glm::vec2 Input::MouseScroll() const {
  if (m_blocked) return {0.0f, 0.0f};
  return m_mousescroll;
}

}  // namespace ptah
