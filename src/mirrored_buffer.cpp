#include "mirrored_buffer.hpp"

namespace ptah {

  MirroredBuffer::MirroredBuffer()
      : m_gpu_buffer{BufferType::UNIFORM}, m_size(0), m_dirty(true) {};

  MirroredBuffer::MirroredBuffer(unsigned int size)
      : m_size(size),
        m_gpu_buffer{BufferType::UNIFORM, size},
        m_buffer(size),
        m_dirty(true) {
    std::fill(m_buffer.begin(), m_buffer.end(), 1);
  };

  void MirroredBuffer::Allocate(unsigned int size) {
    m_size = size;
    m_buffer.resize(size);
    m_gpu_buffer.Resize(size);
    std::fill(m_buffer.begin(), m_buffer.end(), 1);
  };

  void MirroredBuffer::SetData(const void* data, unsigned int size, unsigned int offset) {
    assert(offset + size <= m_buffer.size());
    memcpy(m_buffer.data() + offset, data, size);
    m_dirty = true;
  }

  void MirroredBuffer::Sync() {
    if (m_dirty) {
      PTAH_RENDER_DEBUG("Syncing Mateiral Instance to GPU");
      m_gpu_buffer.SetData(m_buffer.data(), m_buffer.size());
      m_dirty = false;
    }
  }

  DataBuffer& MirroredBuffer::GPUBuffer() { return m_gpu_buffer; }

}