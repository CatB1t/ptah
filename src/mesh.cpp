#include "mesh.hpp"

#include <glad/gl.h>

#include "data_buffer.hpp"

namespace ptah {

void Mesh::m_SetAttribute(int type, unsigned int index, unsigned int count,
                          unsigned int stride, unsigned int offset) {
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, count, type, GL_FALSE, stride, (void*)offset);
}

Mesh::Mesh(const std::vector<Vertex>& vertices)
    : m_count(vertices.size()), m_indexed(false) {
  unsigned int vao = 0;
  glGenVertexArrays(1, &vao);
  m_vao.Set(vao);
  glBindVertexArray(m_vao.Id());

  DataBuffer vertex_buffer{
      BufferType::ARRAY, vertices.data(),
      static_cast<unsigned int>(vertices.size() * sizeof(Vertex))};
  m_SetAttribute(GL_FLOAT, 0, 3, sizeof(Vertex), 0);
  glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int> indices)
    : m_count(indices.size()), m_indexed(true) {
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  m_vao.Set(vao);
  glBindVertexArray(m_vao.Id());

  DataBuffer vertex_buffer{
      BufferType::ARRAY, vertices.data(),
      static_cast<unsigned int>(vertices.size() * sizeof(Vertex))};
  DataBuffer indices_buffer{
      BufferType::ELEMENT, indices.data(),
      static_cast<unsigned int>(indices.size() * sizeof(unsigned int))};
  m_SetAttribute(GL_FLOAT, 0, 3, sizeof(Vertex), 0);
  glBindVertexArray(0);
}

void Mesh::Dispose() {
  // TODO
}

DrawCommand Mesh::GetDrawCommand() const {
  return DrawCommand{m_vao, m_count, m_indexed};
}
}  // namespace ptah
