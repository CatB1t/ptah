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

}  // namespace ptah
