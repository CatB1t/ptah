#pragma once

#include <functional>
#include <string>

namespace ptah {
class Renderer;
class Window;
class MaterialInstance;
class Model;
struct Transform;
}  // namespace ptah

namespace ptah::editor::widgets {
using ModelLoadFn = std::function<void(std::string filepath, bool reposition,
                                       bool resize, bool flip_uv)>;
void ShowOverview(Renderer& renderer, Window& window, double delta_time);
void InspectMaterialInstance(const std::string& label,
                             MaterialInstance& material);
void InspectModel(Model& model, Transform& transform,
                  ModelLoadFn model_load_cb);
}  // namespace ptah::editor::widgets
