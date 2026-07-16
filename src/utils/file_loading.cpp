#include "utils/file_loading.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <fstream>
#include <sstream>

#include "utils/logger.hpp"

namespace ptah {
namespace utils {
std::string load_file(const char* path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    PTAH_RENDER_ERROR("Failed to open file: {}", path);
    return std::string();
  }
  std::ostringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

const aiScene* load_object(Assimp::Importer& importer, const char* path) {
  const aiScene* scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
  if (scene == nullptr) {
    PTAH_RENDER_ERROR("Assimp failed to load model {}", path);
    PTAH_RENDER_ERROR("{}", importer.GetErrorString());
    return nullptr;
  }
  return scene;
}

}  // namespace utils
}  // namespace ptah
