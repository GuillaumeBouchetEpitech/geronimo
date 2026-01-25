
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

namespace gero {
namespace physics {

struct PhysicSixDofConstraintDef {

  enum class RotationOrder : int8_t { XYZ = 0, XZY, YXZ, YZX, ZXY, ZYX };

  BodyWeakRef body_a;
  BodyWeakRef body_b;
  glm::mat4 transform_a;
  glm::mat4 transform_b;
  RotationOrder order = RotationOrder::XYZ;
};

} // namespace physics
} // namespace gero
