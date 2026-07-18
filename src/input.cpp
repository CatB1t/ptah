#include "input.hpp"

#include <GLFW/glfw3.h>

#include "window.hpp"

namespace ptah {

Input::Input() {}

Input::Input(Window& window) { Attach(window); }

void Input::Attach(Window& window) {
  window.AddKeyCallback([&](int key, int action, int scancode) {
    m_keys[key] = (action != GLFW_RELEASE);
  });
}

void Input::Update() { m_keys_prev = m_keys; }

bool Input::IsDown(int key) const { return m_keys[key]; }
bool Input::IsPressed(int key) const {
  return m_keys[key] && !m_keys_prev[key];
}
bool Input::IsReleased(int key) const {
  return !m_keys[key] && m_keys_prev[key];
}

}  // namespace ptah
