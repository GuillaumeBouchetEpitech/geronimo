
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace graphics {

bool sceneToScreen(const glm::vec3& obj,
                   const glm::mat4& modelView,
                   const glm::mat4& projection,
                   const glm::vec2& viewportPos,
                   const glm::vec2& viewportSize,
                   glm::vec3& screenCoord);

}
} // namespace gero
