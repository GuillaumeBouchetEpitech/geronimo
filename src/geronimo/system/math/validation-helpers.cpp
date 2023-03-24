
#include "validation-helpers.hpp"

#include <cmath>

namespace gero {
namespace math {

bool isInvalidFloat(float inVal) { return std::isnan(inVal) || std::isinf(inVal); }

//

bool isInvalidVec2f(const glm::vec2& inVec2) { return isInvalidFloat(inVec2.x) || isInvalidFloat(inVec2.y); }

//

bool isInvalidVec3f(const glm::vec3& inVec3) {
  return isInvalidFloat(inVec3.x) || isInvalidFloat(inVec3.y) || isInvalidFloat(inVec3.z);
}

//

bool isInvalidVec4f(const glm::vec4& inVec4) {
  return isInvalidFloat(inVec4.x) || isInvalidFloat(inVec4.y) || isInvalidFloat(inVec4.z) || isInvalidFloat(inVec4.w);
}

} // namespace math
} // namespace gero
