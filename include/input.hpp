#pragma once

#include <array>
#include <glm/vec2.hpp>

#include "input_keys.hpp"

#define MAX_KEYS_SUPPORT 349
#define MAX_MOUSE_BUTTONS_SUPPORT 8

namespace ptah {
class Window;

class Input {
 private:
  std::array<bool, MAX_KEYS_SUPPORT> m_keys{};
  std::array<bool, MAX_KEYS_SUPPORT> m_keys_prev{};

  std::array<bool, MAX_MOUSE_BUTTONS_SUPPORT> m_mouse_keys{};
  std::array<bool, MAX_MOUSE_BUTTONS_SUPPORT> m_mouse_keys_prev{};
  glm::vec2 m_mousepos{0.0};
  glm::vec2 m_mousepos_prev{0.0};
  glm::vec2 m_mousescroll{0.0};

 public:
  Input();
  Input(Window& window);
  void Attach(Window& window);
  void Update();  // Must be called before PollEvents()

  bool IsDown(int key) const;
  bool IsPressed(int key) const;
  bool IsReleased(int key) const;

  bool IsMouseDown(MouseButton key) const;
  bool IsMousePressed(MouseButton key) const;
  bool IsMouseReleased(MouseButton key) const;
  glm::vec2 MousePosition() const;
  glm::vec2 MouseDelta() const;
  glm::vec2 MouseScroll() const;
};
}  // namespace ptah
