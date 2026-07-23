// Can be overriden on client side
// Here only as a fallback.

#ifndef N_POINT_LIGHTS
#define N_POINT_LIGHTS 4
#endif

struct PointLight {
  vec4 position;
  vec4 color;
};

layout(std140, binding = 0) uniform uFrameData {
  uniform mat4 uView;
  uniform mat4 uProjection;
  uniform mat4 uVP;
  uniform vec4 uDirLightColor;
  uniform vec4 uDirLightDirection;
  PointLight[N_POINT_LIGHTS] uPointLights;
  int uNActivePointLights;
  float uTime;
};
