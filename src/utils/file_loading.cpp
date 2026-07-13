#include "utils/file_loading.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace ptah {
namespace utils {
std::string load_file(const char* path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path << "\n";
    return std::string();
  }
  std::ostringstream contents;
  contents << file.rdbuf();
  return contents.str();
}
}  // namespace utils
}  // namespace ptah
