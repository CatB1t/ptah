#pragma once

#include <functional>
#include <string>
#include <vector>

namespace ptah {
class Renderer;
class Window;
class MaterialInstance;
class Model;
struct Transform;
struct DirectionalLight;
struct PointLight;
}  // namespace ptah

namespace ptah::editor::widgets {
using ModelLoadFn = std::function<void(std::string filepath, bool reposition,
                                       bool resize, bool flip_uv)>;
void ShowOverview(Renderer& renderer, Window& window, double delta_time);
void InspectMaterialInstance(const std::string& label,
                             MaterialInstance& material);
void InspectModel(Model& model, Transform& transform,
                  ModelLoadFn model_load_cb);
void InspectLights(DirectionalLight& dir_light,
                   std::vector<PointLight>& point_lights);
}  // namespace ptah::editor::widgets
