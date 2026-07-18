#pragma once

#include <functional>
#include <vector>

struct GLFWwindow;

namespace ptah {

using WindowResizeFn = std::function<void(unsigned int, unsigned int)>;

class Window {
 private:
  GLFWwindow* m_window;
  const char* m_title;
  unsigned int m_width;
  unsigned int m_height;

  std::vector<WindowResizeFn> m_resize_fns;

 public:
  Window(const char* title, unsigned int width, unsigned int height);
  ~Window();
  bool ShouldClose();
  void Update();
  double Time();
  void AddResizeCallback(WindowResizeFn callback);
};

}  // namespace ptah