#pragma once

#include "core/mesh.hpp"
#include "glm/vec3.hpp"

namespace ptah::primitives {

Mesh MakeCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

Mesh MakeWireframeAABB(const glm::vec3& min, const glm::vec3& max);

}  // namespace ptah::primitives
