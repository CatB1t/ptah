#include "material_props.hpp"

#include <glad/gl.h>

namespace ptah {
int MaterialProps::CullFace() {
  switch (cull_face) {
    case CullFace::Front:
      return GL_FRONT;
    case CullFace::Back:
      return GL_BACK;
    case CullFace::FrontBack:
      return GL_FRONT_AND_BACK;
  }
};

int MaterialProps::DrawMode() {
  switch (draw_mode) {
    case DrawMode::Triangles:
    case DrawMode::Wireframe:
      return GL_TRIANGLES;
    case DrawMode::Lines:
      return GL_LINE_LOOP;
    case DrawMode::Points:
      return GL_POINTS;
  }
};
}  // namespace ptah