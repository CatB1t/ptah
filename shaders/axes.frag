#include "framedata.glsl"
#include "textures.glsl"


in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} fs_in;

out vec4 oColor;

void main() {
  oColor = vec4(normalize(fs_in.normal), 1.0);
}
