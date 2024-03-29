
#pragma once

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/shape/PhysicShapeDef.hpp"

void renderPhysicBody(const gero::physics::PhysicShapeDef& inShapeDef,
                      const glm::vec3& inPosition,
                      const glm::quat& inOrientation);
