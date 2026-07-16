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
  auto& material = renderer.defaultMaterial();

  auto* first_mat = material.createInstance();
  first_mat->SetBlockUniform("color", glm::vec4(1.0, 0.0, 0.0, 1.0));

  auto* second_mat = material.createInstance();
  ptah::MaterialInstance* _mats[2] = {first_mat, second_mat};

  while (!window.ShouldClose()) {
    double time = window.Time();
    renderer.Begin(camera, time);
    const int nSquares = 16;
    for(int i = 0; i < nSquares; i++) {
      glm::mat4 transform {1.0};
      float pi2 = 2.0f * glm::pi<float>();
      float position = (i / (float)nSquares) * pi2 + time;
      transform = glm::translate(transform, glm::vec3(cos(position), sin(position), 0.0));
      transform = glm::rotate(transform, glm::radians(static_cast<float>(time * 90.0f)), glm::vec3(0.0, 0.0, 1.0));
      transform = glm::scale(transform, glm::vec3(0.1, 0.1, 0.1));
      auto& mat = *(_mats[i%2]);
      renderer.Submit(mesh.GetDrawCommand(transform, mat));
    }
    renderer.Flush();
    window.Update();
  }

  exit(EXIT_SUCCESS);
}
