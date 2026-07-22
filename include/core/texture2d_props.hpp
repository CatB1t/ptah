#pragma once

#include <glad/gl.h>

namespace ptah {

enum class MinFilter {
  Linear,
  Nearest,
  LinearLinear,
  LinearNearest,
  NearestLinear,
  NearestNearest
};

enum class MagFilter { Linear, Nearest };

enum class AxisWrap {
  REPEAT,
  REPEAT_MIRROR,
  CLAMP_BORDER,
  CLAMP_EDGE,
  CLAMP_EDGE_MIRROR
};

struct Texture2D_Props {
  MinFilter min_filter = MinFilter::NearestLinear;
  MagFilter mag_filter = MagFilter::Linear;
  AxisWrap s_wrap = AxisWrap::REPEAT;
  AxisWrap t_wrap = AxisWrap::REPEAT;

  int to_gl(MinFilter filter) {
    switch (filter) {
      case MinFilter::Linear:
        return GL_LINEAR;
      case MinFilter::Nearest:
        return GL_NEAREST;
      case MinFilter::LinearLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
      case MinFilter::LinearNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
      case MinFilter::NearestLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
      case MinFilter::NearestNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    }
    return GL_NEAREST_MIPMAP_LINEAR;
  }

  int to_gl(MagFilter filter) {
    switch (filter) {
      case MagFilter::Linear:
        return GL_LINEAR;
      case MagFilter::Nearest:
        return GL_NEAREST;
    }
    return GL_LINEAR;
  }

  int to_gl(AxisWrap wrap) {
    switch (wrap) {
      case AxisWrap::REPEAT:
        return GL_REPEAT;
      case AxisWrap::REPEAT_MIRROR:
        return GL_MIRRORED_REPEAT;
      case AxisWrap::CLAMP_BORDER:
        return GL_CLAMP_TO_BORDER;
      case AxisWrap::CLAMP_EDGE:
        return GL_CLAMP_TO_EDGE;
      case AxisWrap::CLAMP_EDGE_MIRROR:
        return GL_MIRROR_CLAMP_TO_EDGE;
    }
    return GL_REPEAT;
  }
};

}  // namespace ptah
