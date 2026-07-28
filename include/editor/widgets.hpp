#pragma once

namespace ptah {
class Renderer;
class Window;
class MaterialInstance;
}  // namespace ptah

namespace ptah::editor::widgets {
void ShowOverview(Renderer& renderer, Window& window);
void InspectMaterialInstance(MaterialInstance& material);
}  // namespace ptah::editor::widgets
