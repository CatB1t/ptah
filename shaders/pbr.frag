#include "common/framedata.glsl"
#include "common/textures.glsl"
#include "common/fragment_in.glsl"

#define PI 3.14159265

layout(std140, binding = 1) uniform uMaterial {
  vec4 color;
  float roughness;
};

out vec4 oColor;

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

float calculateD(vec3 n, vec3 h) {
    float alpha = roughness * roughness;
    float alpha_sqr = alpha * alpha;
    float nemon = pow(saturate(dot(h, n)), (2.0 / alpha_sqr) - 2.0);
    float denom = 1.0 / PI * alpha_sqr;
    return nemon / denom;
}

float calculateF(vec3 v, vec3 h) {
    float n = 1.2;
    float f_node = pow((n - 1.0), 2) / pow((n + 1.0), 2);
    return f_node + (1.0 - f_node) * pow((1.0 - dot(v, h)), 5.0);
}

float calculateG(vec3 n, vec3 h, vec3 v, vec3 l) {
    float h_n = saturate(dot(h, n));
    float v_h = saturate(dot(v, h));
    float n_v = saturate(dot(n, v));
    float n_l = saturate(dot(n, l));
    float f_or_s = min(2 * h_n * n_v / v_h, 2 * h_n * n_l / v_h);
    return min(1.0, f_or_s);
}

float calculateRs(vec3 n, vec3 l, vec3 v) {
    vec3 h = normalize(v + l);
    float D = calculateD(n, h);
    float G = calculateG(n, h,  v, l);
    float F = calculateF(v, h);
    float denom = 4 * dot(l, n) * dot(v, n);
    return G * F * D / denom;
}

void main() {
  const float kA = 0.1;
  const vec4 ambient_color = vec4(vec3(kA), 1.0);
  vec4 frag_color = color * texture(albedo_tex, fs_in.uv);

  vec3 v = normalize(uViewPosition.xyz - fs_in.position);
  vec3 n = normalize(fs_in.normal);
  vec3 l = uDirLightDirection.xyz;
  vec4 lc = vec4(uDirLightColor.rgb, 1.0);
  float geometry_term = saturate(dot(n, l));

  float s = 1.0;
  float d = 1.0 - s;
  float kd = d;

  float rs = calculateRs(n, l, v);
  vec4 fr = (kd * frag_color / PI) + (s * rs);
  oColor = frag_color * ambient_color + (lc * geometry_term * fr);
}
