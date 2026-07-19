#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <vector>

struct GLFWwindow;

namespace ptah {

using WindowResizeFn = std::function<void(unsigned int, unsigned int)>;
using WindowKeyFn = std::function<void(int key, int action, int mods)>;
using WindowMouseFn = std::function<void(int buttion, int action, int mods)>;
using WindowMousePositionFn = std::function<void(double x, double y)>;
using WindowMouseScrollFn = std::function<void(double xdelta, double ydelta)>;

class Window {
 private:
  GLFWwindow* m_window;
  const char* m_title;
  unsigned int m_width;
  unsigned int m_height;
  double m_prev_time = 0.0;

  std::vector<WindowResizeFn> m_resize_fns;
  std::vector<WindowKeyFn> m_key_fns;
  std::vector<WindowMouseFn> m_mouse_fns;
  std::vector<WindowMousePositionFn> m_mouse_position_fns;
  std::vector<WindowMouseScrollFn> m_mouse_scroll_fns;

 public:
  Window(const char* title, unsigned int width, unsigned int height);
  ~Window();
  void Close();
  bool ShouldClose();
  void PollEvents();
  void SwapBuffers();
  double Time();
  double DeltaTime();
  glm::ivec2 Size() const;
  void AddResizeCallback(WindowResizeFn callback);
  void AddKeyCallback(WindowKeyFn callback);
  void AddMouseCallback(WindowMouseFn callback);
  void AddMousePositionCallback(WindowMousePositionFn callback);
  void AddMouseScrollCallback(WindowMouseScrollFn callback);
};

}  // namespace ptah
