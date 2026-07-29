#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "image.hpp"

struct aiScene;
namespace Assimp {
class Importer;
}

namespace ptah::utils {
std::string load_file(std::filesystem::path path);
const aiScene* load_object(Assimp::Importer& importer, const char* path,
                           unsigned int flags);
std::optional<Image> load_image(std::filesystem::path path);
std::optional<Image> load_image(const std::vector<unsigned char>& image_data);
}  // namespace ptah::utils