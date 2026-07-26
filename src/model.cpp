#include "model.hpp"

#include <assimp/material.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <glm/mat4x4.hpp>

#include "core/material_instance.hpp"
#include "core/texture2d.hpp"
#include "core/texture_slot.hpp"
#include "utils/file_loading.hpp"
#include "utils/logger.hpp"

namespace ptah {

namespace {

std::filesystem::path resolve_texture_path(
    const std::filesystem::path& model_dir,
    const std::filesystem::path& filename) {
  const std::filesystem::path search_dirs[] = {
      model_dir,
      model_dir.parent_path() / "textures",
      model_dir / "textures",
  };
  const std::string extensions[] = {filename.extension().string(), ".png",
                                    ".jpg", ".jpeg"};

  auto default_path = model_dir / filename;
  for (const auto& extension : extensions) {
    auto candidate_name =
        std::filesystem::path(filename).replace_extension(extension);
    for (const auto& dir : search_dirs) {
      auto candidate = dir / candidate_name;
      if (!std::filesystem::exists(candidate)) {
        continue;
      }
      if (candidate != default_path) {
        PTAH_RENDER_WARN("Texture {} not found, falling back to {}",
                         default_path.make_preferred().string(),
                         candidate.make_preferred().string());
      }
      return candidate;
    }
  }
  return default_path;
}

}  // namespace

glm::mat4 assimp_to_glm(aiMatrix4x4& mat) {
  glm::mat4 ret{
      mat.a1, mat.b1, mat.c1, mat.d1, mat.a2, mat.b2, mat.c2, mat.d2,
      mat.a3, mat.b3, mat.c3, mat.d3, mat.a4, mat.b4, mat.c4, mat.d4,
  };
  return ret;
}

Texture2D* Model::m_LoadTexture(const aiMaterial* material,
                                aiTextureType texture_type) {
  aiString path;
  if (aiGetMaterialTexture(material, texture_type, 0, &path) == AI_SUCCESS) {
    auto new_path = resolve_texture_path(
        m_path.parent_path(), std::filesystem::path(path.C_Str()).filename());
    PTAH_RENDER_DEBUG("Loading {} texture: {}", (unsigned int)texture_type,
                      new_path.make_preferred().string().c_str());
    if (auto texture_img = utils::load_image(new_path)) {
      Texture2D* tex = new Texture2D{texture_img.value()};
      m_loaded_textures.push_back(tex);
      return tex;
    }
  }
  return nullptr;
}

MaterialInstance* Model::m_LoadMaterial(const aiScene* scene,
                                        int materialIndex) {
  if (m_loaded_materials.contains(materialIndex)) {
    return m_loaded_materials.at(materialIndex);
  }

  aiMaterial* mat = scene->mMaterials[materialIndex];

  MaterialInstance* instance = m_material.createInstance();
  PTAH_RENDER_DEBUG("Mat[{}] {} :", materialIndex, mat->GetName().C_Str());

  Texture2D* albedo_tex = m_LoadTexture(mat, aiTextureType_BASE_COLOR);
  if (albedo_tex == nullptr) {
    albedo_tex = m_LoadTexture(mat, aiTextureType_DIFFUSE);
  }
  instance->SetTexture(albedo_tex, TextureSlot::Albedo);

  Texture2D* normal_tex = m_LoadTexture(mat, aiTextureType_NORMALS);
  instance->SetTexture(normal_tex, TextureSlot::Normal);

  aiColor4D color;
  if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
    PTAH_RENDER_DEBUG("Diffuse color: {},{},{}", color.r, color.g, color.b);
    instance->SetBlockUniform("color",
                              glm::vec4(color.r, color.g, color.b, color.a));
  }

  m_loaded_materials.insert(std::make_pair(materialIndex, instance));
  return instance;
}

void Model::m_LoadMesh(const aiScene* scene, aiNode* node,
                       glm::mat4 parentTransform) {
  glm::mat4 transformation =
      parentTransform * assimp_to_glm(node->mTransformation);

  for (unsigned int n = 0; n < node->mNumMeshes; n++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[n]];
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      auto position = mesh->mVertices[i];
      auto normal = mesh->mNormals[i];
      aiVector3D uv;
      if (mesh->mTextureCoords[0]) {
        uv = mesh->mTextureCoords[0][i];
      }
      auto tangent = mesh->mTangents[i];

      auto aPosition = glm::vec3(transformation * glm::vec4(position.x, position.y, position.z, 1.0));
      auto aNormal = glm::vec3(normal.x, normal.y, normal.z);
      auto aUV = glm::vec2(uv.x, uv.y);
      auto aTangent = glm::vec3(tangent.x, tangent.y, tangent.z);
      verts.push_back(Vertex{aPosition, aNormal, aUV, aTangent});
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      auto& face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        inds.push_back(face.mIndices[j]);
      }
    }

    MaterialInstance* material = m_LoadMaterial(scene, mesh->mMaterialIndex);

    std::size_t index = m_meshes.size();
    m_meshes.push_back(Mesh{verts, inds});
    m_mesh_materials.insert({index, material});
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    m_LoadMesh(scene, node->mChildren[i], transformation);
  }
}

Model::Model(Material& base_material, const char* filepath)
    : m_material(base_material), m_path(filepath) {
  Assimp::Importer importer;
  const aiScene* scene = utils::load_object(importer, filepath);
  if (scene == nullptr || scene->mRootNode == nullptr) {
    PTAH_RENDER_ERROR("Failed to load model: {}", filepath);
    return;
  }

  PTAH_RENDER_DEBUG("Loading {}", filepath);
  m_LoadMesh(scene, scene->mRootNode, glm::mat4(1.0f));
}

std::vector<DrawCommand> Model::GetDrawCommands(
    const glm::mat4& transform) const {
  std::vector<DrawCommand> commands;
  commands.reserve(m_meshes.size());
  for (std::size_t i = 0; i < m_meshes.size(); i++) {
    MaterialInstance* material = m_mesh_materials.at(i);
    commands.push_back(m_meshes[i].GetDrawCommand(transform, *material));
  }
  return commands;
}

}  // namespace ptah
