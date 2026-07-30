#include "framedata.glsl"
#include "textures.glsl"
#include "fragment_in.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
  float refract_ratio;
  bool b_reflect;
  bool b_refract;
};

out vec4 oColor;

vec4 pointLightsDiffuse(vec3 normal) {
  vec3 pl_diffuse = vec3(0.0);
  for(int i = 0; i < uNActivePointLights; i++) {
      PointLight pl = uPointLights[i];
      vec3 pl_light_dir = normalize(pl.position.xyz - fs_in.position);
      float pl_distance = distance(pl.position.xyz, fs_in.position);
      float attn = 1.0 / (pl_distance * pl_distance);
      pl_diffuse += pl.color.w * attn * pl.color.xyz * max(dot(normal, pl_light_dir), 0.0f);
  }
  return vec4(pl_diffuse, 1.0);
}

void main() {
  vec4 light_color = vec4(uDirLightColor.rgb, 1.0f);
  vec3 n_normal = normalize(fs_in.normal);
  vec3 n_light_dir = uDirLightDirection.xyz;

  float ambient = uDirLightColor.a;
  float diffuse = uDirLightColor.a * max(dot(n_normal, n_light_dir), 0.0f);

  vec4 total_dir_light = (ambient + diffuse) * light_color;
  vec4 total_points_light = pointLightsDiffuse(n_normal);
  vec3 view_dir = normalize(uViewPosition.xyz - fs_in.position); // From fragment to camera
  vec4 texture_color = texture(albedo_tex, fs_in.uv);
  if(b_reflect) {
    vec3 reflected = normalize(reflect(-view_dir, n_normal));
    texture_color = texture(environment_tex, reflected);
  }
  else if(b_refract) {
    vec3 refracted = refract(-view_dir, n_normal, refract_ratio);
    texture_color = texture(environment_tex, refracted);
  }

  oColor = (total_dir_light + total_points_light) * color * texture_color;
}
