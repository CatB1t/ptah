#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>

#include "context.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "window.hpp"

int main() {
  ptah::Context ctx{};
  if (!ctx.IsInitialized()) {
    exit(EXIT_FAILURE);
  }

  ptah::Window window{"Demo", 1280, 720};

  std::vector<ptah::Vertex> vertices{
      {glm::vec3(-0.5, -0.5, 0.0)},
      {glm::vec3(0.5, -0.5, 0.0)},
      {glm::vec3(0.0, 0.5, 0.0)},
  };

  std::vector<unsigned int> indices{
    0, 1, 2,
  };

  ptah::Mesh mesh{vertices, indices};
  ptah::Renderer renderer{1280, 720};
  window.AddResizeCallback([&] (unsigned int width, unsigned int height) {
    renderer.Resize(width, height);
  });
  ptah::Camera camera;
  camera.view = glm::translate(camera.view, glm::vec3(0, 0, -5.0));
  camera.projection = glm::perspective(glm::radians(45.0), 16.0/9.0, 0.01, 100.0);

  auto* material = renderer.defaultMaterial().createInstance();
  material->SetBlockUniform("color", glm::vec4(0.0, 0.5, 0.0, 1.0));

  while (!window.ShouldClose()) {
    window.PollEvents();
    double time = window.Time();
    renderer.Begin(camera, time);
    glm::mat4 transform {1.0f};
    transform = glm::scale(transform, glm::vec3(sin(time) * 0.5f + 1.0f));
    renderer.Submit(mesh.GetDrawCommand(transform, *material));
    renderer.Flush();
    window.SwapBuffers();
  }

  exit(EXIT_SUCCESS);
}
