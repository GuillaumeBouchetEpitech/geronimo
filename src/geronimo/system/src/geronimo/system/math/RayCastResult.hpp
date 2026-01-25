
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

struct RayCastResult {
  float distance{-1.0f};
  glm::vec3 normal{0.0f, 0.0f, 0.0f};
};

} // namespace math
} // namespace gero

