
#include "compute-normal.hpp"

#include "geronimo/system/math/safe-normalize.hpp"

namespace gero {
namespace math {

glm::vec3 computeNormal(const glm::vec3& inPosA, const glm::vec3& inPosB, const glm::vec3& inPosC)
{
  glm::vec3 normal = glm::cross(inPosA - inPosB, inPosA - inPosC);
  gero::math::safeNormalize(normal);
  return normal;
}

}
}
