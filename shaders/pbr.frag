#include "common/framedata.glsl"
#include "common/textures.glsl"
#include "common/fragment_in.glsl"

const float PI = 3.14159265;

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
  float metalness; // 0 -> 1
  float roughness; // 0 -> 1
};

out vec4 oColor;

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

float ndf(float alpha, vec3 n, vec3 h) {
    // Trowbridge-Reitz GGX
    float alpha_sqr = pow(alpha, 2.0);
    float numerator = alpha_sqr;
    float denom = PI * pow((pow(dot(n, h), 2.0) * (alpha_sqr - 1.0) + 1.0), 2.0);
    return numerator / denom;
}

vec3 fresnel(vec3 fo, vec3 v, vec3 h) {
    // How much the material will be reflect, aka Ks
    return fo + (1.0 - fo) * pow((1.0 - dot(v, h)), 5.0);
}

float gschlick(float alpha, vec3 n, vec3 v) {
    float k = pow(alpha + 1, 2.0) / 8.0; // Not same as one used in the UE4 (they used alpha / 2)
    float numerator = saturate(dot(n, v));
    float denom = saturate(dot(n , v)) * (1.0 - k)  + k;
    return numerator / denom;
}

vec3 computeSpecular(float t_roughness, vec3 Ks, vec3 n, vec3 v, vec3 l, vec3 h) {
    // Based on UE4 alpha
    float alpha = pow(t_roughness, 2.0);
    float N = ndf(alpha, n, h);
    float G = gschlick(alpha, n, v) * gschlick(alpha, n, l);
    float denom = 4 * saturate(dot(n, v)) * saturate(dot(n, l));
    return Ks * N * G / (denom + 0.0001);
}


// Out going light dir from fragment to light
vec3 BRDF(float t_rough, float t_metal, vec3 albedo, vec3 N, vec3 V, vec3 light_dir, vec4 light_color) {
  vec3 l = normalize(light_dir);
  vec3 h = normalize(l + V);

  vec3 Fo = vec3(0.04);
  vec3 baseReflectance = mix(Fo, albedo, t_metal);
  vec3 Ks = fresnel(baseReflectance, V, h);
  vec3 Kd = vec3(1.0) - Ks;

  vec3 f_cook = (Kd * albedo / PI) + computeSpecular(t_rough, Ks, N, V, l, h);
  float geometry = saturate(dot(N, l));
  vec3 lc = light_color.rgb * light_color.a;
  vec3 wo = (f_cook * lc * geometry);
  return wo;
}

vec3 apply_normal_mapping() {
  vec3 N = normalize(fs_in.normal);
  // Gram-Schmidt
  vec3 T = normalize(fs_in.tangent - N * dot(N, fs_in.tangent));
  vec3 B = 1.0 * normalize(cross(N, T));
  // TODO: Fix handedness
  mat3 TBN = mat3(T, B, N);  // from tangent space to world space
  vec3 normal_map = texture(normal_tex, fs_in.uv).xyz;
  normal_map = 2.0 * normal_map - 1.0;
  return normalize(TBN * normal_map);
}

void main() {
  // TODO: Flag to use float controls or map
  // TODO: Support IBL
  float t_ao = texture(ao_tex, fs_in.uv).r;
  vec3 albedo = color.rgb * texture(albedo_tex, fs_in.uv).rgb;
  vec3 ambient = t_ao * albedo.rgb * vec3(0.1);

  float t_roughness = texture(roughness_tex, fs_in.uv).g;
  float t_metalness = texture(metalness_tex, fs_in.uv).b;

  vec3 n = apply_normal_mapping();
  vec3 v = normalize(uViewPosition.xyz - fs_in.position);
  vec3 wtotal = vec3(0.0);

  wtotal += BRDF(t_roughness, t_metalness, albedo, n, v, uDirLightDirection.xyz, uDirLightColor);

  for (int i = 0; i < uNActivePointLights; i++) {
    PointLight pl = uPointLights[i];
    vec3 pl_dir = normalize(pl.position.xyz - fs_in.position);
    float pl_distance = distance(pl.position.xyz, fs_in.position);
    float atten = 1.0 / (pl_distance * pl_distance);
    vec4 atten_color = pl.color * vec4(1.0, 1.0, 1.0, atten);
    wtotal += BRDF(t_roughness, t_metalness, albedo, n, v, pl_dir, atten_color);
  }

  vec3 lo = ambient + wtotal;
  oColor = vec4(lo, 1.0);
}
