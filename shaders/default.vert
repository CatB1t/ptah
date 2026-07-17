#include "attrib.glsl"
#include "framedata.glsl"

uniform mat4 uModel;

void main() {
  gl_Position =  uPV * uModel * vec4(aPosition, 1.0);
}
