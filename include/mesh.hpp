#pragma once

#include <vector>

#include "draw_command.hpp"
#include "handle.hpp"
#include "vertex.hpp"

namespace ptah {
class Mesh {
 private:
  MeshHandle m_vao;
  unsigned int m_count;
  bool m_indexed;

 public:
  Mesh(const std::vector<Vertex>& vertices);
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int> indices);
  void Dispose();
  DrawCommand GetDrawCommand() const;
};
}  // namespace ptah