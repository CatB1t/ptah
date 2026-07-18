#pragma once

#include <array>
#include <glm/vec2.hpp>

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
  glm::vec2 m_mousepos;
  glm::vec2 m_mousepos_prev;
  glm::vec2 m_mousescroll;

 public:
  Input();
  Input(Window& window);
  void Attach(Window& window);
  void Update();  // Must be called before PollEvents()

  bool IsDown(int key) const;
  bool IsPressed(int key) const;
  bool IsReleased(int key) const;

  bool IsMouseDown(int key) const;
  bool IsMousePressed(int key) const;
  bool IsMouseReleased(int key) const;
  glm::vec2 MousePosition() const;
  glm::vec2 MouseDelta() const;
  glm::vec2 MouseScroll() const;
};
}  // namespace ptah
