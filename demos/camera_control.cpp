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

  ptah::Model model {renderer, PTAH_ASSETS_DIR "/cube.fbx"};
  glm::mat4 transform{1.0f};

  ptah::OrbitCamera camera{window};
  ptah::Input input_handler{window};
  while (!window.ShouldClose()) {
    input_handler.Update();
    window.PollEvents();
    camera.Update(input_handler);
    double time = window.Time();
    renderer.Begin(camera.Data(), time);
    renderer.Submit(model.GetDrawCommands(transform));
    renderer.Flush();
    window.SwapBuffers();
  }

  exit(EXIT_SUCCESS);
}
