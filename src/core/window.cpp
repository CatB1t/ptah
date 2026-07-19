#include "core/window.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "utils/logger.hpp"

namespace ptah {

Window::Window(const char* title, unsigned int width, unsigned int height)
    : m_title{title}, m_width(width), m_height(height) {
  utils::Logger::Init();
  m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
  if (!m_window) {
    PTAH_RENDER_ERROR("Failed to initialize window {}", m_title);
    return;
  }
  glfwMakeContextCurrent(m_window);
  gladLoadGL(glfwGetProcAddress);

  glfwSetWindowUserPointer(m_window, this);
  glfwSetWindowSizeCallback(m_window, [](GLFWwindow* glfw_window, int width,
                                         int height) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
    window->m_width = width;
    window->m_height = height;
    for (auto& fn : window->m_resize_fns) {
      fn(width, height);
    }
  });

  glfwSetKeyCallback(m_window, [](GLFWwindow* glfw_window, int key,
                                  int scancode, int action, int mods) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
    for (auto& fn : window->m_key_fns) {
      fn(key, action, mods);
    }
  });

  glfwSetMouseButtonCallback(m_window, [](GLFWwindow* glfw_window, int key,
                                          int action, int mods) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
    for (auto& fn : window->m_mouse_fns) {
      fn(key, action, mods);
    }
  });

  glfwSetCursorPosCallback(m_window, [](GLFWwindow* glfw_window, double x,
                                        double y) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
    for (auto& fn : window->m_mouse_position_fns) {
      fn(x, y);
    }
  });

  glfwSetScrollCallback(m_window, [](GLFWwindow* glfw_window, double xdelta,
                                     double ydelta) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
    for (auto& fn : window->m_mouse_scroll_fns) {
      fn(xdelta, ydelta);
    }
  });
}

Window::~Window() { glfwDestroyWindow(m_window); }
void Window::Close() { glfwSetWindowShouldClose(m_window, true); }
bool Window::ShouldClose() { return glfwWindowShouldClose(m_window); }

void Window::PollEvents() { glfwPollEvents(); }

void Window::SwapBuffers() { glfwSwapBuffers(m_window); }

double Window::Time() { return glfwGetTime(); }

void Window::AddResizeCallback(WindowResizeFn callback) {
  m_resize_fns.push_back(callback);
}

void Window::AddKeyCallback(WindowKeyFn callback) {
  m_key_fns.push_back(callback);
}

void Window::AddMouseCallback(WindowMouseFn callback) {
  m_mouse_fns.push_back(callback);
}

void Window::AddMousePositionCallback(WindowMousePositionFn callback) {
  m_mouse_position_fns.push_back(callback);
}

void Window::AddMouseScrollCallback(WindowMouseScrollFn callback) {
  m_mouse_scroll_fns.push_back(callback);
}

double Window::DeltaTime() {
  double current = Time();
  double delta = current - m_prev_time;
  m_prev_time = current;
  return delta;
}

}  // namespace ptah
