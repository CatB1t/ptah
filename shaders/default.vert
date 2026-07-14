#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uProjView;

void main() {
  gl_Position = uProjView * uModel * vec4(position, 1.0);
}
