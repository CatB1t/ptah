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
#include "model.hpp"

int main() {
  ptah::Context ctx{};
  if (!ctx.IsInitialized()) {
    exit(EXIT_FAILURE);
  }

  ptah::Window window{"Demo", 1280, 720};

  ptah::Renderer renderer{1280, 720};
  ptah::Model model {renderer, PTAH_ASSETS_DIR "/cube.fbx"};
  ptah::Camera camera;
  camera.view = glm::translate(camera.view, glm::vec3(0, -2, -20.0));
  camera.projection = glm::perspective(glm::radians(45.0), 16.0/9.0, 0.01, 100.0);

  while (!window.ShouldClose()) {
    double time = window.Time();
    renderer.Begin(camera, time);
    glm::mat4 transform {1.0f};
    transform = glm::rotate(transform, static_cast<float>(time), glm::vec3(0.0, 1.0, 0.0));
    transform = glm::scale(transform, glm::vec3(1.0));
    renderer.Submit(model.GetDrawCommands(transform));
    renderer.Flush();
    window.Update();
  }

  exit(EXIT_SUCCESS);
}
