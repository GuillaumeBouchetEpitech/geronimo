
#pragma once

#include "./RayCastResult.hpp"

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

bool intersectSphere(const glm::vec3& ray_origin,
                     const glm::vec3& ray_direction,
                     const glm::vec3& center,
                     float radius,
                     RayCastResult& outData);

} // namespace math
} // namespace gero
