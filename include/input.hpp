#pragma once

#include <array>

#define MAX_KEYS_SUPPORT 349

namespace ptah {
class Window;

class Input {
 private:
  std::array<bool, MAX_KEYS_SUPPORT> m_keys{};
  std::array<bool, MAX_KEYS_SUPPORT> m_keys_prev{};

 public:
  Input();
  Input(Window& window);
  void Attach(Window& window);
  void Update();  // Must be called before PollEvents()
  bool IsDown(int key) const;
  bool IsPressed(int key) const;
  bool IsReleased(int key) const;
};
}  // namespace ptah
