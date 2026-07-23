#include "utils/file_loading.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#include "utils/logger.hpp"

namespace ptah {
namespace utils {
std::string load_file(std::filesystem::path path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    PTAH_RENDER_ERROR("Failed to open file: {}", path.string());
    return std::string();
  }
  std::ostringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

const aiScene* load_object(Assimp::Importer& importer, const char* path) {
  const aiScene* scene = importer.ReadFile(
      path,
      aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
  if (scene == nullptr) {
    PTAH_RENDER_ERROR("Assimp failed to load model {}", path);
    PTAH_RENDER_ERROR("{}", importer.GetErrorString());
    return nullptr;
  }
  return scene;
}

std::optional<Image> load_image(std::filesystem::path path) {
  int width, height, channels;

  unsigned char* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

  if (!data) {
    auto failure_str = stbi_failure_reason();
    PTAH_RENDER_ERROR("Failed to load image: {}\n{}", path.string().c_str(),
                      failure_str);
    PTAH_RENDER_ERROR("STB: {}", failure_str);
    return {};
  }

  int size = width * height * channels;
  std::vector<unsigned char> img_data(data, data + size);
  stbi_image_free(data);
  auto image_format = channels == 3 ? ImageFormat::RGB : ImageFormat::RGBA;
  return Image{width, height, img_data, image_format};
}

}  // namespace utils
}  // namespace ptah
