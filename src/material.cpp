#include "material.hpp"

#include <glad/gl.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "material_instance.hpp"
#include "utils/file_loading.hpp"
#include "utils/logger.hpp"

namespace ptah {

namespace _utils {

std::filesystem::path get_filepath_from_include(
    std::filesystem::path source_filepath, const std::string& include_line) {
  using namespace std::filesystem;
  int start = include_line.find_first_of('"') + 1;
  int end = include_line.find_last_of('"');
  assert(start != -1 && end != -1);
  std::string file_to_include = include_line.substr(start, end - start);

  auto source_file_dir = source_filepath.parent_path();
  auto full_include_path = source_file_dir / path(file_to_include);
  return full_include_path.make_preferred();
}

std::string preprocess_shader_source(const std::string& source,
                                     std::filesystem::path filepath) {
  std::istringstream source_in(source);
  std::ostringstream source_out;
  std::string temp_line;

  while (std::getline(source_in, temp_line)) {
    if (!temp_line.starts_with("#include")) {
      source_out << temp_line << "\n";
      continue;
    }

    auto include_path = get_filepath_from_include(filepath, temp_line);
    std::string include_source = utils::load_file(include_path);
    include_source = preprocess_shader_source(include_source, include_path);
    source_out << "// #include " << include_path.filename().string() << "\n";
    source_out << include_source << "\n";
    source_out << "// #endinclude " << include_path.filename().string() << "\n";
  }

  return source_out.str();
};

}  // namespace _utils

Material::Material(const char* vertex_filepath, const char* fragment_filepath,
                   const std::vector<std::string>& defines)
    : m_program(glCreateProgram()) {
  unsigned int vertex_id =
      m_LoadShaderSource(vertex_filepath, GL_VERTEX_SHADER, defines);
  unsigned int fragment_id =
      m_LoadShaderSource(fragment_filepath, GL_FRAGMENT_SHADER, defines);
  glAttachShader(m_program.Id(), vertex_id);
  glAttachShader(m_program.Id(), fragment_id);
  glLinkProgram(m_program.Id());
  m_CheckLinkStatus(m_program);
  m_ResolveLayout();
}

unsigned int Material::m_LoadShaderSource(
    std::filesystem::path filepath, unsigned int type,
    const std::vector<std::string>& prepend) {
  unsigned int id = glCreateShader(type);
  const char* type_str = type == GL_VERTEX_ARRAY ? "vertex" : "fragment";

  std::string source = "#version 460 core\n\n";  // Must be first line
  if (prepend.size()) {
    for (const std::string& str : prepend) {
      source += "#define " + str + "\n";
    }
    source += "\n// end prepends \n";
  }

  source +=
      _utils::preprocess_shader_source(utils::load_file(filepath), filepath);
  const int source_len = source.size();
  const char* source_ptr = source.c_str();
  glShaderSource(id, 1, &source_ptr, &source_len);
  glCompileShader(id);
  m_CheckCompileStatus(id, type_str);
  PTAH_RENDER_DEBUG("Loaded shader ({}) source {}:\n{}", type,
                    filepath.string(), source);
  return id;
}

void Material::m_CheckCompileStatus(unsigned int id, const char* type) {
  int status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char error_msg[512];
    PTAH_RENDER_ERROR("Failed to compile shader of type {}", type);
    glGetShaderInfoLog(id, 512, nullptr, error_msg);
    PTAH_RENDER_ERROR(error_msg);
  }
}

void Material::m_CheckLinkStatus(const MaterialHandle& program) {
  int status;
  glGetProgramiv(program.Id(), GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buf[512];
    glGetProgramInfoLog(program.Id(), 512, nullptr, buf);
    PTAH_RENDER_ERROR("Failed to link shader program {}", program.Id());
    PTAH_RENDER_ERROR(buf);
  }
}

void Material::Use() { glUseProgram(m_program.Id()); }

int Material::m_GetUniformLocation(const char* name) {
  int loc = glGetUniformLocation(m_program.Id(), name);
  if (loc < 0) {
    PTAH_RENDER_WARN("Material({}): uniform {} not found, value is ignored.",
                     m_program.Id(), name);
  }
  return loc;
}

void Material::Set(const char* name, const glm::mat4& matrix) {
  int loc = m_GetUniformLocation(name);
  glProgramUniformMatrix4fv(m_program.Id(), loc, 1, GL_FALSE,
                            glm::value_ptr(matrix));
}

void Material::Set(const char* name, const glm::vec4& vec) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform4fv(m_program.Id(), loc, 1, glm::value_ptr(vec));
}

void Material::Set(const char* name, const glm::vec3& vec) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform3fv(m_program.Id(), loc, 1, glm::value_ptr(vec));
}

void Material::Set(const char* name, float value) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform1f(m_program.Id(), loc, value);
}

void Material::Set(const char* name, int value) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform1i(m_program.Id(), loc, value);
}

MaterialBlock Material::m_FetchBlockMetadata() {
  const int program_id = m_program.Id();
  const char* block_name = "uMaterial";

  int block_idx = glGetUniformBlockIndex(program_id, block_name);
  if (block_idx == GL_INVALID_INDEX) {
    PTAH_RENDER_DEBUG("No material block to resolve, should define uMaterial.");
    return {-1, 0};
  }

  int block_size = 0;
  glGetActiveUniformBlockiv(m_program.Id(), block_idx,
                            GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
  return {block_idx, block_size};
}

std::vector<int> Material::m_GetBlockUniformIndices(unsigned int block_index) {
  const int program_id = m_program.Id();
  int n_active_uniforms;
  glGetActiveUniformBlockiv(program_id, block_index,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                            &n_active_uniforms);

  std::vector<int> uniform_indices(n_active_uniforms);
  glGetActiveUniformBlockiv(program_id, block_index,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                            uniform_indices.data());

  return uniform_indices;
}

Layout Material::m_GetUniformLayout(unsigned int uniform_index) {
  const int program_id = m_program.Id();
  int name_length;
  glGetActiveUniformsiv(program_id, 1, &uniform_index, GL_UNIFORM_NAME_LENGTH,
                        &name_length);

  Layout layout;
  layout.name.resize(name_length - 1);
  char* name = layout.name.data();
  glGetActiveUniform(program_id, uniform_index, name_length, nullptr,
                     &layout.length, &layout.type, name);
  glGetActiveUniformsiv(program_id, 1, &uniform_index, GL_UNIFORM_OFFSET,
                        &layout.offset);

  return layout;
}

void Material::m_ResolveLayout() {
  const int program_id = m_program.Id();

  auto material_block = m_FetchBlockMetadata();
  if (material_block.index < 0) {
    return;
  }

  auto uniform_indices = m_GetBlockUniformIndices(material_block.index);
  m_block_size = material_block.size;
  PTAH_RENDER_DEBUG("uMaterial Block [{} bytes] members: {}",
                    material_block.size, uniform_indices.size());

  for (unsigned int uniform_index : uniform_indices) {
    auto layout = m_GetUniformLayout(uniform_index);

    PTAH_RENDER_DEBUG(
        "uMaterial Block [uniform {}]: offset: {}, type: "
        "{:#04x}",
        layout.name, layout.offset, layout.type);

    m_block_uniforms.insert({layout.name, layout});
  }

  m_default_block.resize(m_block_size);
  std::fill(m_default_block.begin(), m_default_block.end(), 0);
}

void Material::Dispose() {
  glDeleteProgram(m_program.Id());
  m_program.Reset();
}

int Material::Size() { return m_block_size; }

MaterialInstance* Material::createInstance() {
  return new MaterialInstance(*this);
}

MaterialProps& Material::Props() { return m_props; }

bool Material::operator<(const Material& other) const {
  return m_program.Id() < other.m_program.Id();
}
}  // namespace ptah