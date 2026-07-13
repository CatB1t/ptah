#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <cstdlib>

#include "context.hpp"
#include "window.hpp"

int main() {
  ptah::Context ctx{};
  if (!ctx.IsInitialized()) {
    exit(EXIT_FAILURE);
  }

  ptah::Window window{"Demo", 1280, 720};
  while (!window.ShouldClose()) {
    window.Update();
  }

  exit(EXIT_SUCCESS);
}
