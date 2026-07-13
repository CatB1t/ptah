#include "mesh.hpp"

#include <glad/gl.h>

namespace ptah {

Mesh::Mesh(const std::vector<Vertex>& vertices)
    : m_count(vertices.size()), m_indexed(false) {
  unsigned int vao = 0;
  glGenVertexArrays(1, &vao);
  m_vao.Set(vao);
  glBindVertexArray(m_vao.Id());

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int> indices)
    : m_count(indices.size()), m_indexed(true) {
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  m_vao.Set(vao);
  glBindVertexArray(m_vao.Id());

  unsigned int buffers[2];
  glGenBuffers(2, buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Dispose() {
  // TODO
}

DrawCommand Mesh::GetDrawCommand() const {
  return DrawCommand{m_vao, m_count, m_indexed};
}
}  // namespace ptah
