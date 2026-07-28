#pragma once

#include <functional>
#include <string>

namespace ptah {
class Renderer;
class Window;
class MaterialInstance;
class Model;
}  // namespace ptah

namespace ptah::editor::widgets {
using ModelLoadFn = std::function<void(std::string filepath, bool reposition,
                                       bool resize, bool flip_uv)>;
void ShowOverview(Renderer& renderer, Window& window);
void InspectMaterialInstance(MaterialInstance& material);
void InspectModel(Model& model, ModelLoadFn model_load_cb);
}  // namespace ptah::editor::widgets
