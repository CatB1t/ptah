#pragma once

#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector>

#include "draw_command.hpp"
#include "mesh.hpp"

struct aiScene;
struct aiNode;

namespace ptah {
class Renderer;
class MaterialInstance;

class Model {
 private:
  std::vector<Mesh> m_meshes;
  std::vector<glm::mat4> m_transforms;
  std::unordered_map<int, MaterialInstance*> m_loaded_materials;
  std::unordered_map<int, MaterialInstance*> m_mesh_materials;
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