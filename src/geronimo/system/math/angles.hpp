
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

float getAngle(float inX, float inY);
float getAngle(const glm::vec2& direction);

glm::vec2 getDirection(float angle);

glm::vec2 rotate2d(const glm::vec2& point, const glm::vec2& center, float angle);

glm::vec3 rotate2d(const glm::vec3& point, const glm::vec2& center, float angle);

float sanitizeAngle(float inRadAngle);

float getDeltaAngleFromAngles(float inCurrentRadAngle, float inDesiredRadAngle);

float getDesiredAngleFromAngle(float inCurrentAngle, float inDesiredAngle, float rotationSpeed);

float getDesiredAngleFromDestination(const glm::vec2& position,
                                     const glm::vec2& target,
                                     float currentAngle,
                                     float rotationSpeed);

} // namespace math
} // namespace gero
