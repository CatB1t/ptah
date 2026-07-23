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

#define MAJOR_INTERVAL 0.2
#define INTERVAL_DIVISIONS 5.0

#define MAJOR_INTENSITY 0.5
#define MINOR_INTENSITY 0.3

#define PI 3.141592653589793238f


// from ShaderToy, temporary to get going.

float grid(float x) {
  float major = smoothstep(0.999, 1.0, cos(x * (2. * PI) / MAJOR_INTERVAL)) * MAJOR_INTENSITY;
  float minor = smoothstep(0.995, 1.0, cos(x * (2. * PI) * INTERVAL_DIVISIONS / MAJOR_INTERVAL)) * MINOR_INTENSITY;
  return max(major, minor);
}

void main()
{
    // Vertically normalized pixel coordinates (from 0 to 1 on vertical only)
    vec3 col = vec3(0.1);
    float xCol = grid(fs_in.uv.x);
    float yCol = grid(fs_in.uv.y);
    col = col + max(xCol, yCol);
    if(col.r <= 0.1)
      discard;
    oColor = vec4(col * vec3(0.5), 1.0);
}