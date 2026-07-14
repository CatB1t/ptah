#version 460 core

out vec4 oColor;

layout(std140, binding = 0) uniform uFrameData {
  uniform mat4 uView;
  uniform mat4 uProjection;
  uniform mat4 uPV;
  float time;
};

void main() {
  oColor = vec4(1.0, 1.0, 0.0, 1.0);
}