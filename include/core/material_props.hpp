#pragma once

namespace ptah {

enum class CullFace { Front, Back, FrontBack };
enum class DrawMode { Triangles, Lines, Wireframe, Points };

struct MaterialProps {
  CullFace cull_face = CullFace::Back;
  DrawMode draw_mode = DrawMode::Triangles;
  bool depth_test = true;
  bool cull = false;

  int CullFace();
  int DrawingMode();
};

}  // namespace ptah