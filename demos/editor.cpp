#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <cstdlib>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/context.hpp"
#include "core/mesh.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "editor/editor_context.hpp"
#include "editor/widgets.hpp"
#include "input.hpp"
#include "model.hpp"
#include "orbit_camera.hpp"

int main() {
  ptah::Context ctx{};
  if (!ctx.IsInitialized()) {
    exit(EXIT_FAILURE);
  }

  ptah::Window window{"Demo", 1920, 1080};
  ptah::Renderer renderer{window};
  ptah::editor::EditorContext editor{window};
  ptah::Material mat = ptah::MakeLambert();
  ptah::Model* model = new ptah::Model{mat, PTAH_ASSETS_DIR "/cube/cube.obj"};

  glm::mat4 transform{1.0f};

  ptah::DirectionalLight dir_light;

  ptah::OrbitCamera camera{window};
  ptah::Input input_handler{window};
  while (!window.ShouldClose()) {
    input_handler.Update();
    window.PollEvents();
    camera.Update(input_handler);
    editor.NewFrame();
    ptah::editor::widgets::ShowOverview(renderer, window);
    ptah::editor::widgets::InspectModel(*model, [&](std::string filepath, bool reposition, bool resize, bool flip_uv) {
      delete model;
      model = new ptah::Model{mat, filepath.c_str(), reposition, resize, flip_uv};
    });

    if (input_handler.IsReleased(ptah::KeyboardKey::Escape)) {
      window.Close();
    }

    double time = window.Time();
    renderer.Begin(camera.Data(), time);
    renderer.Submit(dir_light);

    renderer.Submit(model->GetDrawCommands(transform));
    renderer.Flush();
    editor.Flush();
    window.SwapBuffers();
  }

  exit(EXIT_SUCCESS);
}
