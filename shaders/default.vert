#include "attrib.glsl"
#include "framedata.glsl"
#include "vertex_out.glsl"

uniform mat4 uModel;
uniform mat3 uModelInverse;

void main() {
  gl_Position =  uVP * uModel * vec4(aPosition, 1.0);
  vs_out.position = (uModel * vec4(aPosition, 1.0)).xyz;
  vs_out.normal = uModelInverse * aNormal;
  vs_out.tangent = mat3(uModel) * aTangent;
  vs_out.uv = aUV;
}
