#include "shader.hpp"

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "utils/logger.hpp"

namespace ptah {

Shader::Shader(const std::string& vertex_source,
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

unsigned int Shader::m_LoadShaderSource(const std::string& source,
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

void Shader::m_CheckCompileStatus(unsigned int id, const char* type) {
  int status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char error_msg[512];
    PTAH_RENDER_ERROR("Failed to compile shader of type {}", type);
    glGetShaderInfoLog(id, 512, nullptr, error_msg);
    PTAH_RENDER_ERROR(error_msg);
  }
}

void Shader::m_CheckLinkStatus(const ProgramHandle& program) {
  int status;
  glGetProgramiv(program.Id(), GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buf[512];
    glGetProgramInfoLog(program.Id(), 512, nullptr, buf);
    PTAH_RENDER_ERROR("Failed to link shader program {}", program.Id());
    PTAH_RENDER_ERROR(buf);
  }
}

void Shader::Use() { glUseProgram(m_program.Id()); }

int Shader::m_GetUniformLocation(const char* name) {
  int loc = glGetUniformLocation(m_program.Id(), name);
  if (loc < 0) {
    PTAH_RENDER_WARN("Shader({}): uniform {} not found, value is ignored.",
                     m_program.Id(), name);
  }
  return loc;
}

void Shader::Set(const char* name, const glm::mat4& matrix) {
  int loc = m_GetUniformLocation(name);
  glProgramUniformMatrix4fv(m_program.Id(), loc, 1, GL_FALSE,
                            glm::value_ptr(matrix));
}

void Shader::Set(const char* name, const glm::vec4& vec) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform4fv(m_program.Id(), loc, 1, glm::value_ptr(vec));
}

void Shader::Set(const char* name, const glm::vec3& vec) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform3fv(m_program.Id(), loc, 1, glm::value_ptr(vec));
}

void Shader::Set(const char* name, float value) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform1f(m_program.Id(), loc, value);
}

void Shader::Set(const char* name, int value) {
  int loc = m_GetUniformLocation(name);
  glProgramUniform1i(m_program.Id(), loc, value);
}

void Shader::Dispose() {
  glDeleteProgram(m_program.Id());
  m_program.Reset();
}

}  // namespace ptah