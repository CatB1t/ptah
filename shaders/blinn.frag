#include "common/fragment_in.glsl"
#include "common/framedata.glsl"
#include "common/textures.glsl"

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
  float shininess;
};

out vec4 oColor;

float computeDiffuse(vec3 normal, vec3 light_dir) {
  return max(dot(normal, light_dir), 0.0);
}

float computeSpecular(vec3 normal, vec3 light_dir) {
  vec3 v = normalize(uViewPosition.xyz - fs_in.position);  // World space
  vec3 h = normalize(v + light_dir);
  return max(dot(normal, light_dir), 0.0) *
         pow(clamp(dot(h, normal), 0.0, 1.0), shininess);
}

void main() {
  vec3 N = normalize(fs_in.normal);
  // Gram-Schmidt
  vec3 T = normalize(fs_in.tangent - N * dot(N, fs_in.tangent));
  vec3 B = 1.0 * normalize(cross(N, T));
  // TODO: Fix handedness
  mat3 TBN = mat3(T, B, N);  // from tangent space to world space
  vec3 normal_map = texture(normal_tex, fs_in.uv).xyz;
  normal_map = 2.0 * normal_map - 1.0;
  N = normalize(TBN * normal_map);

  const float ambient = 0.1;
  float diffuse = computeDiffuse(N, uDirLightDirection.xyz);
  float specular = computeSpecular(N, uDirLightDirection.xyz);
  vec3 totalDirLight =
      (ambient + diffuse + specular) * uDirLightColor.xyz * uDirLightColor.w;
  vec3 totalPointLight = vec3(0.0);
  for (int i = 0; i < uNActivePointLights; i++) {
    PointLight light = uPointLights[i];
    vec3 out_dir = normalize(light.position.xyz - fs_in.position);
    float light_distance = length(light.position.xyz - fs_in.position);
    float diffuse = computeDiffuse(N, out_dir);
    float specular = computeSpecular(N, out_dir);
    float atten = 1.0 / (light_distance * light_distance);
    totalPointLight +=
        light.color.rgb * light.color.a * atten * (diffuse + specular);
  }

  vec4 fragment_color = color * texture(albedo_tex, fs_in.uv);
  oColor = vec4(totalPointLight + totalDirLight, 1.0) * fragment_color;
}
