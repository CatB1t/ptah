#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>

#include "core/context.hpp"
#include "core/mesh.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "orbit_camera.hpp"
#include "input.hpp"
#include "model.hpp"

int main() {
  ptah::Context ctx{};
  if (!ctx.IsInitialized()) {
    exit(EXIT_FAILURE);
  }

  ptah::Window window{"Demo", 1280, 720};
  ptah::Renderer renderer{window};

  ptah::Material mat = ptah::MakeLambert();
  ptah::Model model {mat, PTAH_ASSETS_DIR "/backpack/backpack.obj"};
  glm::mat4 transform{1.0f};

  std::vector<ptah::PointLight> lights;
  lights.emplace_back(ptah::constants::colors::RED, glm::vec3(0.0, 0.0, 5.0), 20.0f);
  lights.emplace_back(ptah::constants::colors::GREEN, glm::vec3(0.0, 0.0, -5.0), 20.0f);
  lights.emplace_back(ptah::constants::colors::WHITE, glm::vec3(5.0, 0.0, 0.0), 15.0f);
  lights.emplace_back(ptah::constants::colors::BLUE, glm::vec3(-5.0, 0.0, 0.0), 15.0f);

  ptah::OrbitCamera camera{window};
  ptah::Input input_handler{window};
  while (!window.ShouldClose()) {
    input_handler.Update();
    window.PollEvents();
    camera.Update(input_handler);
    double time = window.Time();
    renderer.Begin(camera.Data(), time);
    renderer.Submit(lights);
    renderer.Submit(model.GetDrawCommands(transform));
    renderer.Flush();
    window.SwapBuffers();
  }

  exit(EXIT_SUCCESS);
}
