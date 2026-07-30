#include "framedata.glsl"
#include "textures.glsl"
#include "fragment_in.glsl"

out vec4 oColor;

void main() {
  oColor = vec4(normalize(fs_in.normal), 1.0);
}
