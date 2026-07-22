#pragma once

#include <assimp/material.h>

#include <filesystem>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector>

#include "core/draw_command.hpp"
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
  std::vector<Mesh> m_meshes;
  std::vector<glm::mat4> m_transforms;
  std::unordered_map<int, MaterialInstance*> m_loaded_materials;
  std::unordered_map<int, MaterialInstance*> m_mesh_materials;
  std::vector<Texture2D*> m_loaded_textures;
  Texture2D* m_LoadTexture(const aiMaterial* material,
                           aiTextureType texture_type);
  MaterialInstance* m_LoadMaterial(Renderer& renderer, const aiScene* scene,
                                   int materialIndex);
  void m_LoadMesh(Renderer& renderer, const aiScene* scene, aiNode* node,
                  glm::mat4 parentTransform);

 public:
  Model(Renderer& renderer, const char* filepath);

  std::vector<DrawCommand> GetDrawCommands(
      const glm::mat4& transform = glm::mat4(1.0f)) const;
};

}  // namespace ptah