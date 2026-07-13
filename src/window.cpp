#include "window.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <iostream>

namespace ptah {

Window::Window(const char* title, unsigned int width, unsigned int height)
    : m_title{title}, m_width(width), m_height(height) {
  m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
  if (!m_window) {
    // TODO: use a better logging system.
    std::cerr << "Window::" << m_title << ": Failed to initialize\n";
    return;
  }
  glfwMakeContextCurrent(m_window);
}

Window::~Window() { glfwDestroyWindow(m_window); }
bool Window::ShouldClose() { return glfwWindowShouldClose(m_window); }
void Window::Update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

}  // namespace ptah