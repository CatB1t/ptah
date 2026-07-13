#pragma once

struct GLFWwindow;

namespace ptah {

class Window {
 private:
  GLFWwindow* m_window;
  const char* m_title;
  unsigned int m_width;
  unsigned int m_height;

 public:
  Window(const char* title, unsigned int width, unsigned int height);
  ~Window();
  bool ShouldClose();
  void Update();
};

}  // namespace ptah