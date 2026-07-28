#include "editor/widgets.hpp"

#include <imgui/imgui.h>

#include "core/renderer.hpp"
#include "core/window.hpp"

namespace ptah::editor::widgets {
void ShowOverview(Renderer& renderer, Window& window) {
  ImGui::Begin("Renderer");
  ImGui::Text("Window size %dx%d", window.Size().x, window.Size().y);
  ImGui::SeparatorText("Renderer Settings");
  ImGui::Checkbox("Grid", &renderer.settings.draw_grid);
  ImGui::Checkbox("Gizmos", &renderer.settings.draw_light_gizmos);
  ImGui::Checkbox("Axes", &renderer.settings.draw_axes);
  ImGui::Checkbox("Skybox", &renderer.settings.draw_skybox);
  ImGui::Checkbox("Override materials", &renderer.settings.override_materials);
  ImGui::Checkbox("Override instances", &renderer.settings.override_instances);
  ImGui::ColorEdit4("Background color", &renderer.settings.background.r);
  ImGui::End();
}

}  // namespace ptah::editor::widgets
