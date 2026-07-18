#pragma once

#include <functional>
#include <vector>

struct GLFWwindow;

namespace ptah {

using WindowResizeFn = std::function<void(unsigned int, unsigned int)>;
using WindowKeyFn = std::function<void(int key, int action, int mods)>;

class Window {
 private:
  GLFWwindow* m_window;
  const char* m_title;
  unsigned int m_width;
  unsigned int m_height;

  std::vector<WindowResizeFn> m_resize_fns;
  std::vector<WindowKeyFn> m_key_fns;

 public:
  Window(const char* title, unsigned int width, unsigned int height);
  ~Window();
  void Close();
  bool ShouldClose();
  void PollEvents();
  void Update();
  double Time();
  void AddResizeCallback(WindowResizeFn callback);
  void AddKeyCallback(WindowKeyFn callback);
};

}  // namespace ptah