#include "data_buffer.hpp"

#include <glad/gl.h>

#include "utils/logger.hpp"

namespace ptah {

unsigned int DataBuffer::m_ResolveType(BufferType type) {
  switch (type) {
    case BufferType::ARRAY:
      return GL_ARRAY_BUFFER;
    case BufferType::ELEMENT:
      return GL_ELEMENT_ARRAY_BUFFER;
    case BufferType::UNIFORM:
      return GL_UNIFORM_BUFFER;
  }
  PTAH_RENDER_ERROR("Invalid buffer type: {}", type);
  return -1;
}

DataBuffer::DataBuffer(BufferType type, unsigned int size) : m_type(type) {
  unsigned int temp_id;
  glGenBuffers(1, &temp_id);
  m_handle = BufferHandle{temp_id};
  GLenum resolved_type = m_ResolveType(type);
  glBindBuffer(resolved_type, m_handle.Id());
  glBufferData(resolved_type, size, nullptr, GL_STATIC_DRAW);
}

DataBuffer::DataBuffer(BufferType type, const void* data, unsigned int size)
    : m_type(type) {
  unsigned int temp_id;
  glGenBuffers(1, &temp_id);
  m_handle = BufferHandle{temp_id};
  GLenum resolved_type = m_ResolveType(type);
  glBindBuffer(resolved_type, m_handle.Id());
  glBufferData(resolved_type, size, data, GL_STATIC_DRAW);
}

void DataBuffer::SetData(const void* data, unsigned int size,
                         unsigned int offset) {
  GLenum resolved_type = m_ResolveType(m_type);
  glBindBuffer(resolved_type, m_handle.Id());
  glBufferSubData(resolved_type, offset, size, data);
}

}  // namespace ptah