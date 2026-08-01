#pragma once

#include <string>
#include <vector>

#include "core/material.hpp"

namespace ptah {

inline Material MakeLambert(const std::vector<std::string>& defines = {}) {
  return Material(PTAH_SHADERS_DIR "/default.vert",
                  PTAH_SHADERS_DIR "/lambert.frag", defines);
}

inline Material MakeUnlit(const std::vector<std::string>& defines = {}) {
  return Material(PTAH_SHADERS_DIR "/default.vert",
                  PTAH_SHADERS_DIR "/unlit.frag", defines);
}

inline Material MakeBlinn(const std::vector<std::string>& defines = {}) {
  auto mat = Material(PTAH_SHADERS_DIR "/default.vert",
                      PTAH_SHADERS_DIR "/blinn.frag", defines);
  mat.SetBlockUniform("shininess", 1.0f);
  return mat;
}

inline Material MakePhong(const std::vector<std::string>& defines = {}) {
  auto mat = Material(PTAH_SHADERS_DIR "/default.vert",
                      PTAH_SHADERS_DIR "/phong.frag", defines);
  mat.SetBlockUniform("shininess", 1.0f);
  return mat;
}

}  // namespace ptah
