#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <cstdlib>

#include "context.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

int main() {
  ptah::Context ctx{};
  if (!ctx.IsInitialized()) {
    exit(EXIT_FAILURE);
  }

  ptah::Window window{"Demo", 1280, 720};

  std::vector<ptah::Vertex> vertices{
      {-0.5, -0.5, 0.0},
      {0.5, -0.5, 0.0},
      {0.0, 0.5, 0.0},
  };
  ptah::Mesh mesh{vertices};
  ptah::Renderer renderer{1280, 720};

  renderer.Submit(mesh.GetDrawCommand());
  while (!window.ShouldClose()) {
    renderer.Flush();
    window.Update();
  }

  exit(EXIT_SUCCESS);
}
