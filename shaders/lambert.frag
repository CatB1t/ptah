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
  vec4 light_color = vec4(uDirLightColor.xyz, 1.0f);
  vec3 n_normal = normalize(fs_in.normal);
  vec3 n_light_dir = uDirLightDirection.xyz;

  float ambient = uDirLightColor.w;
  float diffuse = max(dot(n_normal, n_light_dir), 0.0f);

  vec4 total_light = (ambient + diffuse) * light_color;
  oColor = total_light * color * texture(albedo_tex, fs_in.uv);
}
