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
      {0.5, 0.5, 0.0},
      {-0.5, 0.5, 0.0},
  };

  std::vector<unsigned int> indices{
    0, 1, 2,
    2, 3, 0
  };

  ptah::Mesh mesh{vertices, indices};
  ptah::Renderer renderer{1280, 720};

  while (!window.ShouldClose()) {
    renderer.Submit(mesh.GetDrawCommand());
    renderer.Flush();
    window.Update();
  }

  exit(EXIT_SUCCESS);
}
