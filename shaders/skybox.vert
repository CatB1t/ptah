#include "attrib.glsl"
#include "framedata.glsl"
#include "vertex_out.glsl"

void main() {
  mat4 viewMat = mat4(mat3(uView));
  vec4 position = uProjection * viewMat * vec4(aPosition, 1.0);
  gl_Position = position.xyww;
  vs_out.position = aPosition;
  vs_out.normal = aNormal;
  vs_out.uv = aUV;
}
