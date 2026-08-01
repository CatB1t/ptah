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

void main() {
  // TODO: Support Point Lights
  // TODO: Support IBL
  vec3 t_ao = texture(ao_tex, fs_in.uv).rgb;
  vec3 albedo = color.rgb * texture(albedo_tex, fs_in.uv).rgb;
  vec3 ambient = albedo.rgb * vec3(0.1) * t_ao;

  float t_metalness = texture(metalness_roughness_tex, fs_in.uv).b;
  float t_roughness = texture(metalness_roughness_tex, fs_in.uv).g;

  vec3 n = normalize(fs_in.normal);
  // from frag to light
  vec3 l = normalize(uDirLightDirection.xyz);
  vec3 v = normalize(uViewPosition.xyz - fs_in.position);
  vec3 h = normalize(l + v);

  vec3 Fo = vec3(0.04);
  vec3 baseReflectance = mix(Fo, albedo, t_metalness);
  vec3 Ks = fresnel(baseReflectance, v, h);
  vec3 Kd = vec3(1.0) - Ks;

  vec3 f_cook = (Kd * albedo / PI) + computeSpecular(t_roughness, Ks, n, v, l, h);

  float geometry = saturate(dot(n, l));
  vec3 lc = uDirLightColor.rgb * uDirLightColor.a;
  vec3 wo = ambient + (f_cook * lc * geometry);
  oColor = vec4(wo, 1.0);
}
