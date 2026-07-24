#pragma once

#include <array>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <utility>

#include "core/handle.hpp"
#include "core/material_props.hpp"
#include "texture2d.hpp"
#include "texture_slot.hpp"
#include "utils/logger.hpp"

namespace ptah {

class MaterialInstance;

struct MaterialBlock {
  int index;
  int size;
};

struct Layout {
  std::string name;
  unsigned int type;  // GL Types
  int length;         // Number of elements
  int offset;         // Offset
};

struct ShaderStage {
  std::filesystem::path filepath;
  unsigned int id = 0;
};

class Material {
 private:
  friend class MaterialInstance;
  MaterialHandle m_program;
  std::unordered_map<std::string, Layout> m_block_uniforms;
  int m_block_size = 0;
  std::vector<uint8_t> m_default_block;

  ShaderStage m_vertex;
  ShaderStage m_fragment;
  std::filesystem::file_time_type m_last_modified;
  std::vector<std::string> m_defines;

  MaterialProps m_props;
  std::array<Texture2D*, std::to_underlying(TextureSlot::Count)> m_textures{};

  unsigned int m_LoadShaderSource(std::filesystem::path filepath,
                                  unsigned int type);
  void m_CheckCompileStatus(unsigned int id, const char* type,
                            const std::string& source);
  bool m_IsCompiled(unsigned int id);
  bool m_IsLinked();
  std::filesystem::file_time_type m_LatestWriteTime();
  void m_CheckLinkStatus(const MaterialHandle& program);
  int m_GetUniformLocation(const char* name);

  std::vector<int> m_GetBlockUniformIndices(unsigned int block_index);
  MaterialBlock m_FetchBlockMetadata();
  Layout m_GetUniformLayout(unsigned int uniform_index);
  void m_ResolveLayout();

  static Texture2D* m_texture_defaults[std::to_underlying(TextureSlot::Count)];
  Texture2D* m_ResolveTexture(TextureSlot slot);

 public:
  void static InitDefaults();
  void static DestroyDefaults();

  Material(const char* vertex_filepath, const char* fragment_filepath,
           const std::vector<std::string>& defines = {});
  void Set(const char* name, const glm::mat4& matrix);
  void Set(const char* name, const glm::mat3& matrix);
  void Set(const char* name, const glm::vec4& vec);
  void Set(const char* name, const glm::vec3& vec);
  void Set(const char* name, float value);
  void Set(const char* name, int value);
  void Use();
  void Reload();
  void Dispose();
  int Size();
  void SetTexture(TextureSlot slot, Texture2D* texture);
  MaterialProps& Props();
  MaterialInstance* createInstance();
  bool operator<(const Material& other) const;

  template <typename T>
  void SetBlockUniform(const char* name, const T& data) {
    if (!m_block_uniforms.contains(name)) {
      PTAH_RENDER_WARN("Material block does not contain {}, value is ignored.",
                       name);
      return;
    }

    Layout& layout = m_block_uniforms.at(name);
    PTAH_RENDER_DEBUG("Material ({}): Default uniform {} updated ",
                      m_program.Id(), name);
    memcpy(m_default_block.data() + layout.offset, &data, sizeof(data));
  };
};

}  // namespace ptah
