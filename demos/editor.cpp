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
#include "transform.hpp"

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

  ptah::Transform transform;
  ptah::DirectionalLight dir_light;
  std::vector<ptah::PointLight> lights{};

  ptah::OrbitCamera camera{window};
  ptah::Input input_handler{window};
  while (!window.ShouldClose()) {
    input_handler.Update();
    window.PollEvents();
    editor.NewFrame();
    editor.RouteInput(input_handler);

    double deltaTime = window.DeltaTime();
    if (editor.IsEnabled()) {
      ptah::editor::widgets::ShowOverview(renderer, window, deltaTime);
      ptah::editor::widgets::InspectModel(*model, transform, [&](std::string filepath, bool reposition, bool resize, bool flip_uv) {
        delete model;
        model = new ptah::Model{mat, filepath.c_str(), reposition, resize, flip_uv};
        transform.Reset();
      });
      ptah::editor::widgets::InspectLights(dir_light, lights);
    }

    camera.Update(input_handler);

    if (input_handler.IsPressedRaw(ptah::KeyboardKey::Escape)) {
      window.Close();
    }

    double time = window.Time();
    renderer.Begin(camera.Data(), time);
    renderer.Submit(dir_light);
    renderer.Submit(lights);

    renderer.Submit(model->GetDrawCommands(transform.GetMatrix()));
    renderer.Flush();
    editor.Flush();
    window.SwapBuffers();
  }

  exit(EXIT_SUCCESS);
}
