#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "core/data_buffer.hpp"
#include "utils/logger.hpp"

namespace ptah {

class MirroredBuffer {
 private:
  unsigned int m_size;
  std::vector<uint8_t> m_buffer;
  DataBuffer m_gpu_buffer;
  bool m_dirty;

 public:
  MirroredBuffer();
  MirroredBuffer(unsigned int size);
  MirroredBuffer(const MirroredBuffer& buffer) = delete;
  MirroredBuffer& operator=(const MirroredBuffer& other) = delete;

  DataBuffer& GPUBuffer();
  void Allocate(unsigned int size);
  void SetData(const void* data, unsigned int size, unsigned int offset);
  void Sync();
};

}  // namespace ptah