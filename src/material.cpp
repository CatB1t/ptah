#include "material.hpp"

#include <glad/gl.h>

#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "material_instance.hpp"
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
  m_ResolveLayout();
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

void Material::m_ResolveLayout() {
  int material_block_idx = glGetUniformBlockIndex(m_program.Id(), "uMaterial");
  if (material_block_idx == GL_INVALID_INDEX) {
    PTAH_RENDER_DEBUG("No material block to resolve, should define uMaterial.");
    return;
  }

  const int program_id = m_program.Id();

  glGetActiveUniformBlockiv(m_program.Id(), material_block_idx,
                            GL_UNIFORM_BLOCK_DATA_SIZE, &m_block_size);

  int n_active_uniforms;
  glGetActiveUniformBlockiv(program_id, material_block_idx,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                            &n_active_uniforms);

  std::vector<int> uniform_indices(n_active_uniforms);
  glGetActiveUniformBlockiv(program_id, material_block_idx,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                            uniform_indices.data());

  PTAH_RENDER_DEBUG("uMaterial Block @ {}: Members: {}, Size: {}",
                    material_block_idx, n_active_uniforms, m_block_size);

  for (unsigned int idx : uniform_indices) {
    int name_length;
    glGetActiveUniformsiv(program_id, 1, &idx, GL_UNIFORM_NAME_LENGTH,
                          &name_length);

    Layout layout;
    std::string name_str(name_length - 1, 'x');
    char* name = name_str.data();
    glGetActiveUniform(program_id, idx, name_length, nullptr, &layout.length,
                       &layout.type, name);
    glGetActiveUniformsiv(program_id, 1, &idx, GL_UNIFORM_OFFSET,
                          &layout.offset);

    PTAH_RENDER_DEBUG(
        "uMaterial Block @ {}: [uniform {}]: offset: {}, type: "
        "{:#04x}",
        material_block_idx, name_str, layout.offset, layout.type);

    m_block_uniforms.insert({name_str, layout});
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

}  // namespace ptah