#include "shader.hpp"

#include <glad/gl.h>

#include <cstring>
#include <iostream>

namespace ptah {

Shader::Shader(const std::string& vertex_source,
               const std::string& fragment_source)
    : m_program(glCreateProgram()) {
  unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
  const int vs_len = vertex_source.size();
  const int fs_len = fragment_source.size();
  const char* src_vert = vertex_source.c_str();
  glShaderSource(vertex_id, 1, &src_vert, &vs_len);
  const char* src_frag = fragment_source.c_str();
  glShaderSource(fragment_id, 1, &src_frag, &fs_len);
  glCompileShader(vertex_id);
  char error_msg[512];
  int error_msg_size = 0;
  int status;
  glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    std::cerr << "Failed to compile shader vertex\n";
    glGetShaderInfoLog(vertex_id, 512, &error_msg_size, error_msg);
    std::cerr << error_msg << "\n";
  }

  glCompileShader(fragment_id);

  glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    std::cerr << "Failed to compile shader vertex\n";
    glGetShaderInfoLog(fragment_id, 512, &error_msg_size, error_msg);
    std::cerr << error_msg << "\n";
  }
  glAttachShader(m_program.Id(), vertex_id);
  glAttachShader(m_program.Id(), fragment_id);
  glLinkProgram(m_program.Id());
  glGetProgramiv(m_program.Id(), GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    std::cerr << "Failed to link shader program\n";
  }
}

void Shader::Use() { glUseProgram(m_program.Id()); }

void Shader::Dispose() {
  glDeleteProgram(m_program.Id());
  m_program.Reset();
}

}  // namespace ptah