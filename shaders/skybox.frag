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
  oColor = texture(environment_tex, normalize(fs_in.position)) - 0.4 * vec4(1.0, 1.0, 1.0, 0.0);
}
