#include <filesystem>
#include <string>

#include "image.hpp"

struct aiScene;
namespace Assimp {
class Importer;
}

namespace ptah::utils {
std::string load_file(std::filesystem::path path);
const aiScene* load_object(Assimp::Importer& importer, const char* path);
Image load_image(std::filesystem::path path);
}  // namespace ptah::utils