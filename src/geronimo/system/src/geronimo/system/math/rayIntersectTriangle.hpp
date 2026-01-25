
#pragma once

#include "./RayCastResult.hpp"

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

bool intersectTriangle(
  const glm::vec3& ray_origin,
  const glm::vec3& ray_direction,
  const glm::vec3& v0,
  const glm::vec3& v1,
  const glm::vec3& v2,
  RayCastResult& outData
);

} // namespace math
} // namespace gero

