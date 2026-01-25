
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

namespace gero {
namespace physics {

struct PhysicHingeConstraintDef {
  BodyWeakRef body_a;
  BodyWeakRef body_b;
  glm::vec3 coord_a;
  glm::vec3 coord_b;
  glm::vec3 axis;
};

} // namespace physics
} // namespace gero
