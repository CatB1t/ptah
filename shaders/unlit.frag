#include "framedata.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
};

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} fs_in;

out vec4 oColor;

layout(binding = 0) uniform sampler2D albedo_tex;
layout(binding = 1) uniform sampler2D normal_tex;

void main() {
  oColor = color * texture(albedo_tex, fs_in.uv);
}
