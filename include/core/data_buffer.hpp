#pragma once

#include "core/handle.hpp"

namespace ptah {

enum class BufferType { ARRAY, ELEMENT, UNIFORM };

class DataBuffer {
 private:
  BufferHandle m_handle;
  BufferType m_type;
  unsigned int m_ResolveType(BufferType type);

 public:
  DataBuffer(BufferType type);
  DataBuffer(BufferType type, unsigned int size);
  DataBuffer(BufferType type, const void* data, unsigned int size);
  void Resize(unsigned int size);
  void SetData(const void* data, unsigned int size, unsigned int offset = 0);
  void BindUniform(
      unsigned int index);  // TODO: may not the best place to put it here?
};
}  // namespace ptah