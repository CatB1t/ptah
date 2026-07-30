#include "model.hpp"

#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "core/helpers/primitives.hpp"
#include "core/material_instance.hpp"
#include "core/texture2d.hpp"
#include "core/texture_slot.hpp"
#include "materials/materials_factory.hpp"
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

Texture2D* Model::m_LoadTexture(const aiScene* scene,
                                const aiMaterial* material,
                                std::vector<aiTextureType> texture_types) {
  aiString path;
  for (auto texture_type : texture_types) {
    if (aiGetMaterialTexture(material, texture_type, 0, &path) == AI_SUCCESS) {
      if (auto texture = scene->GetEmbeddedTexture(path.C_Str());
          texture != nullptr) {
        if (texture->mHeight == 0) {
          std::vector<unsigned char> data;
          data.resize(texture->mWidth);
          memcpy(data.data(), texture->pcData, texture->mWidth);
          if (auto texture_img = utils::load_image(data)) {
            PTAH_RENDER_DEBUG("Loading embedded texture: {}.{}({} bytes)",
                              texture->mFilename.C_Str(),
                              texture->achFormatHint, texture->mWidth);
            Texture2D* tex = new Texture2D{texture_img.value()};
            textures.push_back(tex);
            return tex;
          }
        }
      } else {
        auto new_path = resolve_texture_path(
            m_path.parent_path(),
            std::filesystem::path(path.C_Str()).filename());
        PTAH_RENDER_DEBUG("Loading {} texture: {}", (unsigned int)texture_type,
                          new_path.make_preferred().string().c_str());
        if (auto texture_img = utils::load_image(new_path)) {
          Texture2D* tex = new Texture2D{texture_img.value()};
          textures.push_back(tex);
          return tex;
        }
      }
    }
  }
  return nullptr;
}

MaterialInstance* Model::m_LoadMaterial(const aiScene* scene,
                                        int materialIndex) {
  aiMaterial* mat = scene->mMaterials[materialIndex];
  auto name = mat->GetName();
  std::string str_name = name.length ? name.C_Str() : "<unknown>";

  MaterialInstance* instance = material.createInstance();
  PTAH_RENDER_DEBUG("Mat[{}] {} :", materialIndex, str_name);

  Texture2D* albedo_tex = m_LoadTexture(
      scene, mat, {aiTextureType_BASE_COLOR, aiTextureType_DIFFUSE});
  instance->SetTexture(albedo_tex, TextureSlot::Albedo);

  Texture2D* normal_tex = m_LoadTexture(scene, mat, {aiTextureType_NORMALS});
  instance->SetTexture(normal_tex, TextureSlot::Normal);

  aiColor4D color;
  if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
    PTAH_RENDER_DEBUG("Diffuse color: {},{},{}", color.r, color.g, color.b);
    instance->SetBlockUniform("color",
                              glm::vec4(color.r, color.g, color.b, color.a));
  }

  material_instances[materialIndex] = std::make_pair(str_name, instance);
  return instance;
}

void Model::m_UpdateBoundingBox(const glm::vec3& vertex_position) {
  m_min_pos.x = glm::min(vertex_position.x, m_min_pos.x);
  m_min_pos.y = glm::min(vertex_position.y, m_min_pos.y);
  m_min_pos.z = glm::min(vertex_position.z, m_min_pos.z);

  m_max_pos.x = glm::max(vertex_position.x, m_max_pos.x);
  m_max_pos.y = glm::max(vertex_position.y, m_max_pos.y);
  m_max_pos.z = glm::max(vertex_position.z, m_max_pos.z);
}

void Model::m_LoadMesh(const aiScene* scene, aiNode* node,
                       glm::mat4 parentTransform) {
  glm::mat4 transformation =
      parentTransform * assimp_to_glm(node->mTransformation);

  glm::mat3 normal_transform =
      glm::transpose(glm::inverse(glm::mat3(transformation)));

  for (unsigned int n = 0; n < node->mNumMeshes; n++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[n]];
    auto name = mesh->mName;
    std::string str_name = name.length ? name.C_Str() : "<unknown>";
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      auto position = mesh->mVertices[i];
      auto normal = mesh->mNormals[i];
      aiVector3D uv;
      aiVector3D tangent;
      if (mesh->mTextureCoords[0]) {
        uv = mesh->mTextureCoords[0][i];
      }
      if (mesh->mTangents) {
        tangent = mesh->mTangents[i];
      }

      auto aPosition = glm::vec3(
          transformation * glm::vec4(position.x, position.y, position.z, 1.0));

      auto aNormal = normal_transform * glm::vec3(normal.x, normal.y, normal.z);
      auto aUV = glm::vec2(uv.x, uv.y);
      auto aTangent = glm::mat3(transformation) *
                      glm::vec3(tangent.x, tangent.y, tangent.z);
      verts.push_back(Vertex{aPosition, aNormal, aUV, aTangent});
      m_UpdateBoundingBox(aPosition);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      auto& face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        inds.push_back(face.mIndices[j]);
      }
    }

    MaterialInstance* mat_instance =
        material_instances[mesh->mMaterialIndex].second;

    std::size_t index = meshes.size();
    meshes.push_back({str_name, Mesh{verts, inds}});
    mesh_materials.insert({index, mat_instance});
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    m_LoadMesh(scene, node->mChildren[i], transformation);
  }
}

Model::Model(Material& base_material, const char* filepath, bool reposition,
             bool resize, bool flip_uvs)
    : m_path(filepath),
      // TODO: Very wasteful allocation here, for each model.
      m_bb_material(MakeUnlit()),
      m_bb_material_instance(m_bb_material.createInstance()),
      material(base_material) {
  unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
  if (flip_uvs) flags |= aiProcess_FlipUVs;

  Assimp::Importer importer;
  const aiScene* scene = utils::load_object(importer, filepath, flags);
  if (scene == nullptr || scene->mRootNode == nullptr) {
    PTAH_RENDER_ERROR("Failed to load model: {}", filepath);
    return;
  }

  PTAH_RENDER_DEBUG("Loading {}", filepath);

  material_instances.resize(scene->mNumMaterials);
  for (int i = 0; i < material_instances.size(); i++) {
    m_LoadMaterial(scene, i);
  }

  m_LoadMesh(scene, scene->mRootNode, glm::mat4(1.0f));
  m_bounding_box =
      new Mesh{primitives::MakeWireframeAABB(m_min_pos, m_max_pos)};
  m_bb_material.props.draw_mode = DrawMode::Lines;
  m_bb_material_instance->SetBlockUniform("color", glm::vec3(0.0f, 0.5f, 0.0f));

  if (reposition || resize) {
    auto delta = m_max_pos - m_min_pos;

    if (resize) {
      const float target_size = 5.0f;  // 5 units
      float scale_ratio = target_size / glm::length(delta);
      m_internal_transform =
          glm::scale(m_internal_transform, glm::vec3{scale_ratio});
    }

    if (reposition) {
      m_internal_transform = glm::translate(
          m_internal_transform, -1.0f * (m_min_pos + (delta / 2.0f)));
      m_internal_transform = glm::translate(
          m_internal_transform, glm::vec3(0.0, 1.0, 0.0) * (delta / 2.0f));
    }
  }
}

std::vector<DrawCommand> Model::GetDrawCommands(
    const glm::mat4& transform) const {
  std::vector<DrawCommand> commands;
  commands.reserve(meshes.size());

  auto world = transform * m_internal_transform;

  for (std::size_t i = 0; i < meshes.size(); i++) {
    MaterialInstance* mat_instance = mesh_materials.at(i);
    commands.push_back(meshes[i].second.GetDrawCommand(world, *mat_instance));
  }

  if (draw_bounding_box) commands.push_back(m_GetBoundingBox(world));

  return commands;
}

DrawCommand Model::m_GetBoundingBox(const glm::mat4& transform) const {
  return m_bounding_box->GetDrawCommand(transform, *m_bb_material_instance);
}

}  // namespace ptah
