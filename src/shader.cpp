#include "shader.hpp"

#include <glad/gl.h>

#include <cstring>
#include <iostream>

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
    int error_msg_size = 0;
    std::cerr << "Failed to compile shader of type " << type << ":\n";
    glGetShaderInfoLog(id, 512, &error_msg_size, error_msg);
    std::cerr << error_msg_size << error_msg[0] << "\n";
  }
}

void Shader::m_CheckLinkStatus(const Handle& program) {
  int status;
  glGetProgramiv(program.Id(), GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buf[512];
    glGetProgramInfoLog(program.Id(), 512, nullptr, buf);
    std::cerr << "Failed to link shader program: " << buf << "\n";
  }
}

void Shader::Use() { glUseProgram(m_program.Id()); }

void Shader::Dispose() {
  glDeleteProgram(m_program.Id());
  m_program.Reset();
}

}  // namespace ptah