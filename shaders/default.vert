#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTangent;

uniform mat4 uModel;

layout(std140, binding = 0) uniform uFrameData {
  uniform mat4 uView;
  uniform mat4 uProjection;
  uniform mat4 uPV;
  float time;
};

void main() {
  gl_Position =  uPV * uModel * vec4(aPosition, 1.0);
}
