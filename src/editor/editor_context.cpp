#include "editor/editor_context.hpp"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include "core/window.hpp"
#include "input.hpp"

namespace ptah::editor {
EditorContext::EditorContext(Window& window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window.RawWindow(), true);
  ImGui_ImplOpenGL3_Init();
}

void EditorContext::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void EditorContext::Flush() {
  if (!m_enabled) {
    ImGui::EndFrame();
    return;
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorContext::SetEnabled(bool enabled) { m_enabled = enabled; }
bool EditorContext::IsEnabled() const { return m_enabled; }
void EditorContext::Toggle() { SetEnabled(!m_enabled); }

void EditorContext::RouteInput(Input& input, KeyboardKey key) {
  if (!ImGui::GetIO().WantTextInput && input.IsPressedRaw(key)) Toggle();
  input.SetBlocked(m_enabled);
}

EditorContext::~EditorContext() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
}  // namespace ptah::editor
