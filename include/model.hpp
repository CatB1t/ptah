#pragma once

#include <assimp/material.h>

#include <filesystem>
#include <glm/mat4x4.hpp>
#include <limits>
#include <unordered_map>
#include <vector>

#include "core/draw_command.hpp"
#include "core/material.hpp"
#include "core/mesh.hpp"
#include "core/texture2d.hpp"

struct aiScene;
struct aiNode;

namespace ptah {
class Renderer;
class MaterialInstance;

class Model {
 private:
  std::filesystem::path m_path;
  glm::mat4 m_internal_transform{1.0f};

  Material m_bb_material;
  MaterialInstance* m_bb_material_instance;
  Mesh* m_bounding_box;
  glm::vec3 m_min_pos{std::numeric_limits<float>::max()};
  glm::vec3 m_max_pos{std::numeric_limits<float>::lowest()};

  Texture2D* m_LoadTexture(const aiMaterial* material,
                           aiTextureType texture_type);
  MaterialInstance* m_LoadMaterial(const aiScene* scene, int materialIndex);
  void m_LoadMesh(const aiScene* scene, aiNode* node,
                  glm::mat4 parentTransform);
  void m_UpdateBoundingBox(const glm::vec3& vertex_position);
  DrawCommand m_GetBoundingBox(
      const glm::mat4& transform = glm::mat4(1.0f)) const;

 public:
  Material& material;
  std::vector<std::pair<std::string, Mesh>> meshes;
  std::vector<Texture2D*> textures;
  std::unordered_map<int, MaterialInstance*> mesh_materials;
  std::vector<std::pair<std::string, MaterialInstance*>> material_instances;
  bool draw_bounding_box = false;

  Model(Material& base_material, const char* filepath, bool reposition = false,
        bool resize = false, bool flip_uvs = true);

  std::vector<DrawCommand> GetDrawCommands(
      const glm::mat4& transform = glm::mat4(1.0f)) const;
};

}  // namespace ptah