#include <string>

struct aiScene;
namespace Assimp {
class Importer;
}

namespace ptah::utils {
std::string load_file(const char* path);
const aiScene* load_object(Assimp::Importer& importer, const char* path);
}  // namespace ptah::utils