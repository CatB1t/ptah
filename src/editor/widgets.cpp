#include "editor/widgets.hpp"

#include <imgui/imgui.h>
#include <portable_file_dialogs.h>

#include <format>
#include <glm/common.hpp>
#include <utility>

#include "core/material_instance.hpp"
#include "core/material_props.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "model.hpp"
#include "transform.hpp"
#include "utils/file_loading.hpp"
#include "utils/logger.hpp"

// This is a bit messy. I'm not a UI specialist
// but this is good enough for the purpose of debugging
// this is not an actual editor for the engine

namespace {
using namespace ptah;

void ShowTexture2D(const char* label, MaterialInstance& mat, TextureSlot slot) {
  auto* texture = static_cast<Texture2D*>(mat.GetTexture(slot));
  ImTextureRef texture_ref;
  if (texture != nullptr) {
    texture_ref = texture->Handle().Id();
  }

  ImGui::Separator();
  if (ImGui::ImageButton(label, texture_ref, ImVec2(64, 64), ImVec2(0.0f, 0.0f),
                         ImVec2(1.0f, 1.0f))) {
    auto files =
        pfd::open_file("Load Texture", ".",
                       {"Texture", "*.png *.jpeg *.jpg", "All Files", "*"})
            .result();
    if (files.size() > 0) {
      auto image = utils::load_image(files[0]);
      if (image) {
        auto texture = new Texture2D(*image);
        mat.SetTexture(texture, slot);
      }
    }
  }
  ImGui::PushID(label);
  ImGui::SameLine();
  ImGui::Text("%s", label);
  ImGui::SameLine(0, 10);
  if (ImGui::Button("Clear Texture")) {
    mat.SetTexture(nullptr, slot);
  }
  ImGui::PopID();
}

void UniformFloat4(MaterialInstance& material, Layout& layout) {
  float* colors = material.View<float>(layout.name.c_str());
  if (ImGui::ColorEdit4(layout.name.c_str(), colors)) {
    material.SetBlockUniform(
        layout.name.c_str(),
        glm::vec4(colors[0], colors[1], colors[2], colors[3]));
  }
}

void UniformFloat(MaterialInstance& material, Layout& layout) {
  float* flag = material.View<float>(layout.name.c_str());
  if (ImGui::DragFloat(layout.name.c_str(), flag, 0.05f, 1.0f, 2.0f)) {
    material.SetBlockUniform(layout.name.c_str(), *flag);
  }
}

void UniformBool(MaterialInstance& material, Layout& layout) {
  bool* flag = material.View<bool>(layout.name.c_str());
  if (ImGui::Checkbox(layout.name.c_str(), flag)) {
    material.SetBlockUniform(layout.name.c_str(), *flag);
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
void ShowOverview(Renderer& renderer, Window& window, double delta_time) {
  ImGui::Begin("Renderer");
  ImGui::Text("Window size %dx%d", window.Size().x, window.Size().y);
  ImGui::Text("Frametime %lfms", delta_time * 1000.0);
  ImGui::Text("FPS %lf", 1.0 / delta_time);
  ImGui::SeparatorText("Renderer Settings");
  ImGui::Checkbox("Grid", &renderer.settings.draw_grid);
  ImGui::Checkbox("Gizmos", &renderer.settings.draw_light_gizmos);
  ImGui::Checkbox("Axes", &renderer.settings.draw_axes);
  ImGui::Checkbox("Skybox", &renderer.settings.draw_skybox);
  ImGui::Checkbox("Override materials", &renderer.settings.override_materials);
  ImGui::Checkbox("Override instances", &renderer.settings.override_instances);
  ImGui::ColorEdit4("Background color", &renderer.settings.background.r);
  if (ImGui::Button("Load skybox")) {
    auto ret = pfd::select_folder("Skybox", ".");
    if (ret.result() != "") {
      TextureCube skybox{ret.result()};
      renderer.environment.SetSkybox(skybox);
    }
  };
  ImGui::End();
}

void InspectMaterialInstance(const std::string& label,
                             MaterialInstance& material) {
  ImGui::Begin(std::format("Material ({})###MaterialEditor", label).c_str());
  if (ImGui::CollapsingHeader("Base Properties")) {
    auto& base_props = material.Base().props;
    ::ShowMaterialProps(base_props);
  }

  auto layouts = material.Base().GetLayout();
  ImGui::SeparatorText("Uniforms");
  for (auto& [name, layout] : layouts) {
    if (layout.type_name == "fvec4") {
      UniformFloat4(material, layout);
    }

    if (layout.type_name == "bool") {
      UniformBool(material, layout);
    }

    if (layout.type_name == "float") {
      UniformFloat(material, layout);
    }
  }

  ImGui::SeparatorText("Textures");
  ShowTexture2D("Albedo", material, TextureSlot::Albedo);
  ShowTexture2D("Normal", material, TextureSlot::Normal);
  ImGui::End();
}

void InspectModel(Model& model, Transform& transform,
                  ModelLoadFn model_load_cb) {
  ImGui::Begin("Model");
  ImGui::SeparatorText("Properties");
  ImGui::Checkbox("Draw Bounding Box", &model.draw_bounding_box);
  ImGui::Spacing();
  ImGui::Text("# Meshes: %zu", model.meshes.size());
  ImGui::Text("# Materials: %zu", model.material_instances.size());
  ImGui::Text("# Textures: %zu", model.textures.size());

  ImGui::SeparatorText("Transform");
  ImGui::DragFloat3("Position", &transform.position.x, 1.0f, 0.0f, 0.0f, "%.3f",
                    ImGuiSliderFlags_ColorMarkers);
  ImGui::DragFloat3("Rotation", &transform.rotation.x, 1.0f, 0.0f, 0.0f, "%.3f",
                    ImGuiSliderFlags_ColorMarkers);
  ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f, 0.0f, 0.0f, "%.3f",
                    ImGuiSliderFlags_ColorMarkers);
  if (ImGui::Button("Reset")) {
    transform.Reset();
  }

  ImGui::Separator();
  static Model* last_selected_model = nullptr;
  static int selected_instance = 0;
  static int selected_mesh = 0;
  if (last_selected_model != &model) {
    selected_instance = 0;
    selected_mesh = 0;
    last_selected_model = &model;
  }

  if (ImGui::BeginListBox("Meshes")) {
    for (int n = 0; n < model.meshes.size(); n++) {
      const bool is_selected = (selected_mesh == n);
      if (ImGui::Selectable(model.meshes[n].first.c_str(), is_selected)) {
        selected_mesh = n;
        for (int i = 0; i < model.material_instances.size(); i++) {
          auto pair = model.material_instances[i];
          if (pair.second == model.mesh_materials.at(selected_mesh)) {
            selected_instance = i;
            break;
          }
        }
      }

      // Set the initial focus when opening the combo (scrolling + keyboard
      // navigation focus)
      if (is_selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndListBox();
  }

  if (ImGui::BeginListBox("Material Instances")) {
    for (int n = 0; n < model.material_instances.size(); n++) {
      const bool is_selected = (selected_instance == n);
      if (ImGui::Selectable(model.material_instances[n].first.c_str(),
                            is_selected)) {
        selected_instance = n;
      }

      // Set the initial focus when opening the combo (scrolling + keyboard
      // navigation focus)
      if (is_selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndListBox();
  }

  InspectMaterialInstance(model.material_instances[selected_instance].first,
                          *model.material_instances[selected_instance].second);

  static std::string selected_model = "";

  if (ImGui::Button("Load model")) {
    auto files =
        pfd::open_file("Load Model", ".",
                       {"Model", "*.obj *.fbx *.gltf *.glb", "All Files", "*"})
            .result();
    if (files.size() > 0) {
      selected_model = files[0];
      ImGui::OpenPopup("ModelLoadOptions");
    }
  }

  if (ImGui::BeginPopupModal("ModelLoadOptions", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    static bool reposition = false;
    static bool resize = false;
    static bool flip_uvs = true;
    auto reset_vars = [&]() {
      reposition = false;
      resize = false;
      flip_uvs = true;
    };

    ImGui::Checkbox("Reposition", &reposition);
    ImGui::Checkbox("Rescale", &resize);
    ImGui::Checkbox("Flip UVs", &flip_uvs);

    if (ImGui::Button("Import", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
      model_load_cb(selected_model, reposition, resize, flip_uvs);
      reset_vars();
    }

    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
      reset_vars();
    }
    ImGui::EndPopup();
  }

  ImGui::End();
}
}  // namespace ptah::editor::widgets
