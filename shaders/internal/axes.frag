#include "../common/framedata.glsl"
#include "../common/textures.glsl"
#include "../common/fragment_in.glsl"

out vec4 oColor;

void main() {
  oColor = vec4(normalize(fs_in.normal), 1.0);
}
