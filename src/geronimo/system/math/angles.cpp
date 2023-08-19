
#include "angles.hpp"

#include "constants.hpp"
#include "clamp.hpp"

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
  // bring the delta value to the [0..pi2] range
  while (inRadAngle >= +math::pi2)
    inRadAngle -= math::pi2;
  while (inRadAngle < 0.0f)
    inRadAngle += math::pi2;
  return inRadAngle;
}

float sanitizeDeltaAngle(float inRadAngle) {
  // bring the delta value to the [-pi..pi] range
  while (inRadAngle > +math::pi)
    inRadAngle -= math::pi2;
  while (inRadAngle < -math::pi)
    inRadAngle += math::pi2;
  return inRadAngle;
}

float sanitizeAngle2(float inRadAngle) {
  // bring the delta value to the [-pi2..pi2] range
  while (inRadAngle >= +math::pi2)
    inRadAngle -= math::pi2;
  while (inRadAngle <= -math::pi2)
    inRadAngle += math::pi2;
  return inRadAngle;
}

float getDeltaAngleFromAngles(float inCurrentRadAngle, float inDesiredRadAngle) {
  const float safeDesiredRadAngle = sanitizeAngle(inDesiredRadAngle);
  const float safeCurrentRadAngle = sanitizeAngle(inCurrentRadAngle);
  return sanitizeDeltaAngle(safeDesiredRadAngle - safeCurrentRadAngle);
}

float getDesiredDeltaFromDelta(float inDeltaRadAngle, float inRadRotationSpeed) {
  float outDeltaRadAngle = inDeltaRadAngle;

  if (outDeltaRadAngle > 0.0f) {
    // limit
    outDeltaRadAngle = std::min(outDeltaRadAngle, +inRadRotationSpeed);
  } else if (outDeltaRadAngle < 0.0f) {
    // limit
    outDeltaRadAngle = std::max(outDeltaRadAngle, -inRadRotationSpeed);
  }

  return outDeltaRadAngle;
}

float getDesiredAngleFromDelta(float inCurrentRadAngle, float inDeltaRadAngle, float inRadRotationSpeed) {
  const float outDeltaRadAngle = getDesiredDeltaFromDelta(inDeltaRadAngle, inRadRotationSpeed);
  return sanitizeDeltaAngle(sanitizeAngle2(inCurrentRadAngle + outDeltaRadAngle));
}

float getDesiredAngleFromAngle(float inCurrentRadAngle, float inDesiredRadAngle, float inRadRotationSpeed) {
  const float deltaRadAngle = getDeltaAngleFromAngles(inCurrentRadAngle, inDesiredRadAngle);
  return getDesiredAngleFromDelta(inCurrentRadAngle, deltaRadAngle, inRadRotationSpeed);
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
