#include "editor/widgets.hpp"

#include <imgui/imgui.h>

#include <glm/common.hpp>
#include <utility>

#include "core/material_instance.hpp"
#include "core/material_props.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"

namespace {
using namespace ptah;

void ShowTexture2D(const char* label, Texture2D* texture) {
  if (texture == nullptr) return;

  ImGui::SeparatorText(label);
  ImGui::Image(texture->Handle().Id(), ImVec2(128, 128), ImVec2(0.0f, 0.0f),
               ImVec2(1.0f, 1.0f));
}

void UniformFloat4(MaterialInstance& material, Layout& layout) {
  float* colors = material.View<float>(layout.name.c_str());
  if (ImGui::ColorEdit4(layout.name.c_str(), colors)) {
    material.SetBlockUniform(
        layout.name.c_str(),
        glm::vec4(colors[0], colors[1], colors[2], colors[3]));
  }
}

void ShowMaterialProps(MaterialProps& props) {
  ImGui::Checkbox("Depth test", &props.depth_test);
  ImGui::Checkbox("Depth write", &props.depth_write);
  ImGui::Checkbox("Cull", &props.cull);

  const char* options[static_cast<int>(CullFace::Count)]{"Front", "Back",
                                                         "FrontBack"};
  int current = std::to_underlying(props.cull_face);
  if (ImGui::Combo("Culling Face", &current, options,
                   static_cast<int>(CullFace::Count))) {
    props.cull_face = static_cast<CullFace>(current);
  }

  const char* options_draw[static_cast<int>(DrawMode::Count)]{
      "Triangles", "Lines", "Wireframe", "Points"};
  int current_draw = std::to_underlying(props.draw_mode);
  if (ImGui::Combo("Drawing Mode", &current_draw, options_draw,
                   static_cast<int>(DrawMode::Count))) {
    props.draw_mode = static_cast<DrawMode>(current_draw);
  }
}
}  // namespace

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

void InspectMaterialInstance(MaterialInstance& material) {
  ImGui::Begin("Material");
  auto& base_props = material.Base().props;
  ::ShowMaterialProps(base_props);
  auto layouts = material.Base().GetLayout();
  ImGui::SeparatorText("Material Uniforms: ");
  for (auto& [name, layout] : layouts) {
    if (layout.type_name == "fvec4") {
      UniformFloat4(material, layout);
    }
  }

  auto* tex = static_cast<Texture2D*>(material.GetTexture(TextureSlot::Albedo));
  ShowTexture2D("Albedo", tex);
  tex = static_cast<Texture2D*>(material.GetTexture(TextureSlot::Normal));
  ShowTexture2D("Normal", tex);
  ImGui::End();
}
}  // namespace ptah::editor::widgets
