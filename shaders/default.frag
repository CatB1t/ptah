#version 460 core

out vec4 oColor;

layout(std140, binding = 0) uniform uFrameData {
  uniform mat4 uView;
  uniform mat4 uProjection;
  uniform mat4 uPV;
  float time;
};

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
};

void main() {
  oColor = vec4(color);
}