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

Mesh MakeWireframeAABB(const glm::vec3& min, const glm::vec3& max) {
  std::vector<Vertex> vertices{
      {{min.x, min.y, min.z}}, {{max.x, min.y, min.z}},
      {{max.x, max.y, min.z}}, {{min.x, max.y, min.z}},

      {{min.x, min.y, max.z}}, {{max.x, min.y, max.z}},
      {{max.x, max.y, max.z}}, {{min.x, max.y, max.z}},
  };

  std::vector<unsigned int> indices;
  indices.reserve(24);
  for (unsigned int corner = 0; corner < 4; ++corner) {
    const unsigned int next = (corner + 1) % 4;
    indices.insert(indices.end(), {
                                      corner,
                                      next,  // back ring
                                      corner + 4,
                                      next + 4,  // front ring
                                      corner,
                                      corner + 4,  // connector
                                  });
  }

  return Mesh{vertices, indices};
}

}  // namespace ptah::primitives
