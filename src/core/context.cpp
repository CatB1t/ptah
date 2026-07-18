#include "core/context.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "utils/logger.hpp"

namespace ptah {

bool Context::m_initialized = false;

Context::Context() {
  if (Context::m_initialized) return;

  if (!glfwInit()) {
    PTAH_RENDER_ERROR("Failed to initialize GLFW");
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  Context::m_initialized = true;
}

bool Context::IsInitialized() { return Context::m_initialized; }

Context::~Context() {
  if (!Context::m_initialized) return;
  glfwTerminate();
}
}  // namespace ptah