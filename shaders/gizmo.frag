#include "framedata.glsl"
#include "textures.glsl"
#include "fragment_in.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
};

out vec4 oColor;

void main() {
  oColor = color * texture(albedo_tex, fs_in.uv);

  if(oColor.a <= 0.001)
    discard;
}
