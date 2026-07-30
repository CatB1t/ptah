#include "framedata.glsl"
#include "textures.glsl"
#include "fragment_in.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
};

out vec4 oColor;

void main() {
  oColor = texture(environment_tex, normalize(fs_in.position));
}
