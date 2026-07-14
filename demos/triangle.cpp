#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
  ptah::Camera camera;
  camera.view = glm::translate(camera.view, glm::vec3(0, 0, -5.0));
  camera.projection = glm::perspective(glm::radians(45.0), 16.0/9.0, 0.01, 100.0);

  while (!window.ShouldClose()) {
    renderer.Begin(camera);
    renderer.Submit(mesh.GetDrawCommand());
    renderer.Flush();
    window.Update();
  }

  exit(EXIT_SUCCESS);
}
