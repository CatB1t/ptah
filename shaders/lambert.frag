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

vec4 pointLightsDiffuse(vec3 normal) {
  vec3 pl_diffuse = vec3(0.0);
  for(int i = 0; i < uNActivePointLights; i++) {
      PointLight pl = uPointLights[i];
      vec3 pl_light_dir = normalize(pl.position.xyz - fs_in.position);
      pl_diffuse += pl.color.w * pl.color.xyz * max(dot(normal, pl_light_dir), 0.0f);
  }
  return vec4(pl_diffuse, 1.0);
}

void main() {
  vec4 light_color = vec4(uDirLightColor.xyz, 1.0f);
  vec3 n_normal = normalize(fs_in.normal);
  vec3 n_light_dir = uDirLightDirection.xyz;

  float ambient = uDirLightColor.w;
  float diffuse = uDirLightColor.w * max(dot(n_normal, n_light_dir), 0.0f);

  vec4 total_dir_light = (ambient + diffuse) * light_color;
  vec4 total_points_light = pointLightsDiffuse(n_normal);
  oColor = (total_dir_light + total_points_light) * color * texture(albedo_tex, fs_in.uv);
}
