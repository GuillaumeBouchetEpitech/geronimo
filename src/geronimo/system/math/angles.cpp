
#include "angles.hpp"

#include "constants.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <cmath>

namespace gero {
namespace math {

float getAngle(float inX, float inY) { return std::atan2(inY, inX); }

float getAngle(const glm::vec2& direction) { return getAngle(direction.x, direction.y); }

glm::vec2 getDirection(float angle) { return glm::vec2(std::cos(angle), std::sin(angle)); }

glm::vec2 rotate2d(const glm::vec2& point, const glm::vec2& center, float angle) {
  const glm::vec2 dir = getDirection(angle);
  const glm::vec2 diff = point - center;

  return glm::vec2(center.x + diff.x * dir.x + diff.y * dir.y, center.y + diff.x * dir.y - diff.y * dir.x);
}

glm::vec3 rotate2d(const glm::vec3& point, const glm::vec2& center, float angle) {
  return glm::vec3(rotate2d(glm::vec2(point), center, angle), point.z);
}

float sanitizeAngle(float inRadAngle) {
  // bring the delta value to the [-pi..pi] range
  while (inRadAngle >= +math::pi2)
    inRadAngle -= math::pi2;
  while (inRadAngle < 0.0f)
    inRadAngle += math::pi2;

  return inRadAngle;
}

float getDeltaAngleFromAngles(float inCurrentRadAngle, float inDesiredRadAngle) {
  float deltaRadAngle = sanitizeAngle(inDesiredRadAngle) - sanitizeAngle(inCurrentRadAngle);

  // bring the delta value to the [-pi..pi] range
  while (deltaRadAngle >= +math::pi)
    deltaRadAngle -= math::pi2;
  while (deltaRadAngle <= -math::pi)
    deltaRadAngle += math::pi2;

  return deltaRadAngle;
}

float getDesiredAngleFromAngle(float inCurrentRadAngle, float inDesiredRadAngle, float inRadRotationSpeed) {

  //
  //
  // get delta angle

  float deltaRadAngle = getDeltaAngleFromAngles(inCurrentRadAngle, inDesiredRadAngle);

  if (deltaRadAngle > 0.0f) {
    // limit
    if (deltaRadAngle > +inRadRotationSpeed)
      deltaRadAngle = +inRadRotationSpeed;
  } else if (deltaRadAngle < 0.0f) {
    // limit
    if (deltaRadAngle < -inRadRotationSpeed)
      deltaRadAngle = -inRadRotationSpeed;
  }

  //
  //
  // determine final angle

  return inCurrentRadAngle + deltaRadAngle;
}

float getDesiredAngleFromDestination(const glm::vec2& position,
                                     const glm::vec2& target,
                                     float currentAngle,
                                     float inRadRotationSpeed) {

  const float desiredAngle = gero::math::getAngle(target - position);

  return gero::math::getDesiredAngleFromAngle(currentAngle, desiredAngle, inRadRotationSpeed);
}

} // namespace math
} // namespace gero
