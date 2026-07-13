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
    // TODO: use a better logging system.
    PTAH_RENDER_ERROR("Failed to initialize window {}", m_title);
    return;
  }
  glfwMakeContextCurrent(m_window);
  gladLoadGL(glfwGetProcAddress);
}

Window::~Window() { glfwDestroyWindow(m_window); }
bool Window::ShouldClose() { return glfwWindowShouldClose(m_window); }
void Window::Update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

}  // namespace ptah