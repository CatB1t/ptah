#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ptah::constants {

inline constexpr float PI = glm::pi<float>();

// Right handed
inline constexpr glm::vec3 UP{0.0f, 1.0f, 0.0f};
inline constexpr glm::vec3 DOWN{0.0f, -1.0f, 0.0f};
inline constexpr glm::vec3 RIGHT{1.0f, 0.0f, 0.0f};
inline constexpr glm::vec3 LEFT{-1.0f, 0.0f, 0.0f};
inline constexpr glm::vec3 FORWARD{0.0f, 0.0f, -1.0f};
inline constexpr glm::vec3 BACKWARD{0.0f, 0.0f, 1.0f};

namespace colors {
inline constexpr glm::vec4 WHITE{1.0f, 1.0f, 1.0f, 1.0f};
inline constexpr glm::vec4 BLACK{0.0f, 0.0f, 0.0f, 1.0f};
inline constexpr glm::vec4 RED{1.0f, 0.0f, 0.0f, 1.0f};
inline constexpr glm::vec4 GREEN{0.0f, 1.0f, 0.0f, 1.0f};
inline constexpr glm::vec4 BLUE{0.0f, 0.0f, 1.0f, 1.0f};
}  // namespace colors

}  // namespace ptah::constants