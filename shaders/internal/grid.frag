#include "../common/framedata.glsl"
#include "../common/textures.glsl"
#include "../common/fragment_in.glsl"


layout(std140, binding = 1) uniform uMaterial { vec4 color; };

out vec4 oColor;

float line_test(float target, float x) {
  const float thickness = 0.001;
  return step(target - thickness, x) * (1.0 - step(target + thickness, x));
}

void main() {
  // TODO: Properly optimize?
  vec4 c = vec4(0.0);
  float u = fs_in.uv.x;
  float v = fs_in.uv.y;
  float origin_test = clamp(line_test(0.5, v) + line_test(0.5, u), 0.0, 1.0);
  c = line_test(0.5, v) * vec4(0.5, 0.0, 0.0, 1.0);
  c += line_test(0.5, u) * vec4(0.0, 0.0, 0.5, 1.0);
  vec4 flat_line_c = vec4(0.1, 0.1, 0.1, 1.0);
  int max_steps = 20;
  float step_size = 1.0 / max_steps;
  for (int i = 0; i <= max_steps; i++) {
    float x = float(i) * step_size;
    c += clamp(line_test(x, u) + line_test(x, v) - origin_test, 0.0, 1.0) *
         flat_line_c;
  }
  if (c.a < 0.5) {
    discard;
  }
  oColor = c;
}
