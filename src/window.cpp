#include "window.hpp"

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

}  // namespace ptah
