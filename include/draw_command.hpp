#pragma once

#include "handle.hpp"

namespace ptah {
struct DrawCommand {
  MeshHandle handle;
  unsigned int count;
  bool indexed;
};
}  // namespace ptah
