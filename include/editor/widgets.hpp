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
void ShowOverview(Renderer& renderer, Window& window);
void InspectMaterialInstance(MaterialInstance& material);
void InspectModel(Model& model, std::function<void(std::string)> model_load_cb);
}  // namespace ptah::editor::widgets
