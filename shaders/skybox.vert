#include "attrib.glsl"
#include "framedata.glsl"

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} vs_out;

void main() {
  mat4 viewMat = mat4(mat3(uView));
  gl_Position = uProjection * viewMat * vec4(aPosition, 1.0);
  vs_out.position = aPosition;
  vs_out.normal = aNormal;
  vs_out.uv = aUV;
}
