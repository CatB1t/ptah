#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "data_buffer.hpp"
#include "handle.hpp"
#include "utils/logger.hpp"

namespace ptah {

struct Layout {
  unsigned int type;  // GL Types
  int length;         // Number of elements
  int offset;         // Offset
};

struct _SyncBuffer {
  unsigned int size = 0;
  std::vector<uint8_t> buffer;
  DataBuffer gpu_buffer;
  bool is_dirty = true;

  _SyncBuffer() : gpu_buffer{BufferType::UNIFORM} {};
  _SyncBuffer(unsigned int size)
      : size(size), gpu_buffer{BufferType::UNIFORM, size}, buffer(size) {
    std::fill(buffer.begin(), buffer.end(), 1);
  };

  void Allocate(unsigned int size) {
    this->size = size;
    buffer.resize(size);
    gpu_buffer.Resize(size);
    std::fill(buffer.begin(), buffer.end(), 1);
  };

  void SetData(const void* data, unsigned int size, unsigned int offset) {
    assert(offset + size <= buffer.size());
    memcpy(buffer.data() + offset, data, size);
    is_dirty = true;
  }

  void Sync() {
    if (is_dirty) {
      PTAH_RENDER_DEBUG("Syncing Mateiral Instance to GPU");
      gpu_buffer.SetData(buffer.data(), buffer.size());
      is_dirty = false;
    }
  }
};

class Material {
 private:
  friend class MaterialInstance;
  MaterialHandle m_program;
  std::unordered_map<std::string, Layout> m_block_uniforms;
  unsigned int m_block_size = 0;
  std::vector<uint8_t> m_default_block;

  unsigned int m_LoadShaderSource(const std::string& source, unsigned int type);
  void m_CheckCompileStatus(unsigned int id, const char* type);
  void m_CheckLinkStatus(const MaterialHandle& program);
  int m_GetUniformLocation(const char* name);
  void m_ResolveLayout();

 public:
  Material(const std::string& vertex_source,
           const std::string& fragment_source);
  void Set(const char* name, const glm::mat4& matrix);
  void Set(const char* name, const glm::vec4& vec);
  void Set(const char* name, const glm::vec3& vec);
  void Set(const char* name, float value);
  void Set(const char* name, int value);
  void Use();
  void Dispose();
  unsigned int Size();
  MaterialInstance* createInstance();

  template <typename T>
  void SetBlockUniform(const char* name, const T& data) {
    if (!m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN("Material block does not contain {}, value is ignored.",
                       name);
      return;
    }

    Layout& layout = m_block_uniforms.at(name);
    PTAH_RENDER_DEBUG("Material ({}): Default uniform {} updated ", m_program.Id(), name);
    memcpy(m_default_block.data() + layout.offset, &data, sizeof(data));
  };
};

}  // namespace ptah
