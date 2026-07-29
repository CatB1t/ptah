#pragma once

#include "core/window.hpp"
#include "input_keys.hpp"

namespace ptah {
class Input;
}

namespace ptah::editor {
class EditorContext {
 private:
  bool m_enabled = false;

 public:
  EditorContext(Window& window);
  EditorContext(const EditorContext& other) = delete;
  EditorContext& operator=(const EditorContext& other) = delete;
  void NewFrame();
  void Flush();

  void SetEnabled(bool enabled);
  bool IsEnabled() const;
  void Toggle();
  void RouteInput(Input& input, KeyboardKey key = KeyboardKey::E);

  ~EditorContext();
};
}  // namespace ptah::editor
