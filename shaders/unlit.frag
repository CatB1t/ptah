#include "framedata.glsl"
#include "textures.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
};

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} fs_in;

out vec4 oColor;

void main() {
  oColor = color * texture(albedo_tex, fs_in.uv);
}
