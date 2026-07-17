out vec4 oColor;

#include "framedata.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
};

void main() {
  oColor = vec4(color);
}