#pragma once

namespace ptah {

enum class CullFace { Front, Back, FrontBack, Count };
enum class DrawMode { Triangles, Lines, Wireframe, Points, Count };

struct MaterialProps {
  CullFace cull_face = CullFace::Back;
  DrawMode draw_mode = DrawMode::Triangles;
  bool depth_test = true;
  bool depth_write = true;
  bool cull = false;

  int CullingFace();
  int DrawingMode();
};

}  // namespace ptah