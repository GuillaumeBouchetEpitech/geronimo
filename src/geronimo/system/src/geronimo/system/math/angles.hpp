
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

float getAngle(float inX, float inY);
float getAngle(const glm::vec2& direction);

void getEulerAngles(const glm::vec3& inInputVector, float& outYaw, float& outPitch);
glm::vec3 getForwardFromEuler(float inYaw, float inPitch);

glm::vec2 getDirection(float angle);

glm::vec2 rotate2d(const glm::vec2& point, const glm::vec2& center, float angle);

glm::vec3 rotate2d(const glm::vec3& point, const glm::vec2& center, float angle);

float sanitizeAngle(float inRadAngle);
float sanitizeDeltaAngle(float inRadAngle);

float getDeltaAngleFromAngles(float inCurrentRadAngle, float inDesiredRadAngle);

float getDesiredDeltaFromDelta(float inDeltaRadAngle, float inRadRotationSpeed);

float getDesiredAngleFromDelta(float inCurrentRadAngle, float inDeltaRadAngle, float inRadRotationSpeed);

float getDesiredAngleFromAngle(float inCurrentRadAngle, float inDesiredRadAngle, float inRadRotationSpeed);

float getDesiredAngleFromDestination(const glm::vec2& position,
                                     const glm::vec2& target,
                                     float currentAngle,
                                     float rotationSpeed);

} // namespace math
} // namespace gero
