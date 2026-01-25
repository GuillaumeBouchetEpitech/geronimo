
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

bool isInvalidFloat(float inVal);
bool isInvalidVec2f(const glm::vec2& inVec2);
bool isInvalidVec3f(const glm::vec3& inVec3);
bool isInvalidVec4f(const glm::vec4& inVec4);

} // namespace math
} // namespace gero
