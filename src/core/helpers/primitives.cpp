#include "core/helpers/primitives.hpp"

#include <vector>

namespace ptah::primitives {

// Box centered on the origin, faces wound CCW as seen from outside.
// Tangents are left at their default value.
Mesh MakeCube(float width, float height, float depth) {
  const float x = width * 0.5f;
  const float y = height * 0.5f;
  const float z = depth * 0.5f;

  std::vector<Vertex> vertices{
      // front (+Z)
      {{-x, -y, z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
      {{x, -y, z}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
      {{x, y, z}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-x, y, z}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

      // back (-Z)
      {{x, -y, -z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
      {{-x, -y, -z}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
      {{-x, y, -z}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
      {{x, y, -z}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

      // right (+X)
      {{x, -y, z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{x, -y, -z}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{x, y, -z}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
      {{x, y, z}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

      // left (-X)
      {{-x, -y, -z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{-x, -y, z}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{-x, y, z}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
      {{-x, y, -z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

      // top (+Y)
      {{-x, y, z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{x, y, z}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{x, y, -z}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      {{-x, y, -z}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

      // bottom (-Y)
      {{-x, -y, -z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
      {{x, -y, -z}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
      {{x, -y, z}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
      {{-x, -y, z}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
  };

  std::vector<unsigned int> indices;
  indices.reserve(36);
  for (unsigned int face = 0; face < 6; ++face) {
    const unsigned int base = face * 4;
    indices.insert(indices.end(),
                   {base, base + 1, base + 2, base + 2, base + 3, base});
  }

  return Mesh{vertices, indices};
}

}  // namespace ptah::primitives
