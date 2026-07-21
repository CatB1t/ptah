#include "attrib.glsl"
#include "framedata.glsl"

uniform mat4 uModel;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} vs_out;

void main() {
  gl_Position =  uVP * uModel * vec4(aPosition, 1.0);
  vs_out.position = (uModel * vec4(aPosition, 1.0)).xyz;
  vs_out.normal = transpose(inverse(mat3(uModel))) * aNormal;
  vs_out.uv = aUV;
}
