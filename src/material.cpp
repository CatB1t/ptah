#include "material.hpp"

#include <glad/gl.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "utils/logger.hpp"

namespace ptah {

Material::Material(const std::string& vertex_source,
                   const std::string& fragment_source)
    : m_program(glCreateProgram()) {
  unsigned int vertex_id = m_LoadShaderSource(vertex_source, GL_VERTEX_SHADER);
  unsigned int fragment_id =
      m_LoadShaderSource(fragment_source, GL_FRAGMENT_SHADER);
  glAttachShader(m_program.Id(), vertex_id);
  glAttachShader(m_program.Id(), fragment_id);
  glLinkProgram(m_program.Id());
  m_CheckLinkStatus(m_program);
}

unsigned int Material::m_LoadShaderSource(const std::string& source,
                                          unsigned int type) {
  unsigned int id = glCreateShader(type);
  const char* type_str = type == GL_VERTEX_ARRAY ? "vertex" : "fragment";
  const int source_len = source.size();
  const char* source_ptr = source.c_str();
  glShaderSource(id, 1, &source_ptr, &source_len);
  glCompileShader(id);
  m_CheckCompileStatus(id, type_str);
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

void Material::ResolveLayout() {
  int materialBlockIdx = glGetUniformBlockIndex(m_program.Id(), "uMaterial");
  if (materialBlockIdx == GL_INVALID_INDEX) {
    PTAH_RENDER_DEBUG("No material block to resolve.");
    return;
  }

  int length;
  glGetActiveUniformBlockiv(m_program.Id(), materialBlockIdx,
                            GL_UNIFORM_BLOCK_NAME_LENGTH, &length);

  char* temp = new char[length];
  glGetActiveUniformBlockName(m_program.Id(), materialBlockIdx, length, nullptr,
                              temp);

  int blockSize = 0;
  glGetActiveUniformBlockiv(m_program.Id(), materialBlockIdx,
                            GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

  int active_uniforms;
  glGetActiveUniformBlockiv(m_program.Id(), materialBlockIdx,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniforms);

  int* uniform_indices = new int[active_uniforms];
  glGetActiveUniformBlockiv(m_program.Id(), materialBlockIdx,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                            uniform_indices);
  std::stringstream ss;

  for (size_t i = 0; i < active_uniforms; ++i) {
    ss << uniform_indices[i];
    if (i < active_uniforms - 1) {
      ss << ", ";  // Optional delimiter
    }
  }

  PTAH_RENDER_DEBUG(
      "Block {}: Name: {}, Max Length: {}, Members: {}, Indices: {}, Size: {}",
      materialBlockIdx, temp, length, active_uniforms, ss.str(), blockSize);

  for (int j = 0; j < active_uniforms; j++) {
    int name_length;
    unsigned int idx = uniform_indices[j];
    glGetActiveUniformsiv(m_program.Id(), 1, &idx, GL_UNIFORM_NAME_LENGTH,
                          &name_length);

    Layout layout;
    layout.name.resize(name_length);
    char* name = layout.name.data();
    glGetActiveUniform(m_program.Id(), uniform_indices[j], 512, nullptr,
                       &layout.length, &layout.type, name);
    glGetActiveUniformsiv(m_program.Id(), 1, &idx, GL_UNIFORM_OFFSET,
                          &layout.offset);

    PTAH_RENDER_DEBUG(
        "Block {} > Uniform {}: Name: {}, Offset: {}, Type: "
        "{:#04x}",
        materialBlockIdx, j, layout.name, layout.offset, layout.type);
  }

  delete temp;
  delete uniform_indices;
}

void Material::Dispose() {
  glDeleteProgram(m_program.Id());
  m_program.Reset();
}

}  // namespace ptah