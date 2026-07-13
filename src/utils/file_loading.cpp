#include "utils/file_loading.hpp"

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
}  // namespace utils
}  // namespace ptah
