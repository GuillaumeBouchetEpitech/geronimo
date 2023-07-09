
#include "safe-normalize.hpp"

namespace gero {
namespace math {

float safeNormalize(glm::vec3& inVec3)
{
  const float magnitude = glm::length(inVec3);
  if (magnitude > 0.0f) {
    inVec3 /= magnitude;
  }
  return magnitude;
}

}
}
