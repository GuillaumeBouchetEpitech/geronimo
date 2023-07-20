
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

namespace gero {
namespace physics {

struct PhysicSixDofConstraintDef {
  BodyWeakRef body_a;
  BodyWeakRef body_b;
  glm::vec3 center;
  // glm::vec3 anchor;
  // glm::vec3 axis1;
  // glm::vec3 axis2;
};

} // namespace physics
} // namespace gero
