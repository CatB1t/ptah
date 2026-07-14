#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 uModel;

layout(std140, binding = 0) uniform uFrameData {
  uniform mat4 uView;
  uniform mat4 uProjection;
  uniform mat4 uPV;
  float time;
};

void main() {
  gl_Position =  uPV * uModel * vec4(position, 1.0);
}
