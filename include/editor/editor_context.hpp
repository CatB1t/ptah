#pragma once

#include "core/window.hpp"

namespace ptah::editor {
class EditorContext {
 public:
  EditorContext(Window& window);
  EditorContext(const EditorContext& other) = delete;
  EditorContext& operator=(const EditorContext& other) = delete;
  void NewFrame();
  void Flush();
  ~EditorContext();
};
}  // namespace ptah::editor
