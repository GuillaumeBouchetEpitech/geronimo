
#include "rayIntersectSphere.hpp"

#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace math {

bool intersectSphere(const glm::vec3& ray_origin,
                     const glm::vec3& ray_direction,
                     const glm::vec3& center,
                     float radius,
                     RayCastResult& outData) {
  glm::vec3 m = ray_origin - center;
  float b = glm::dot(m, ray_direction);
  float c = glm::dot(m, m) - radius * radius;

  // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
  if (c > 0 && b > 0) {
    return false;
  }
  float discr = b * b - c;

  // A negative discriminant corresponds to ray missing sphere
  if (discr < 0.0) {
    return false;
  }

  // Ray now found to intersect sphere, compute smallest t value of intersection
  float outT = -b - std::sqrt(discr);

  // If t is negative, ray started inside sphere so clamp t to zero
  if (outT < 0.0) {
    outT = 0.0;
  }
  // q = p + outT * d;

  outData.distance = outT;
  outData.normal = glm::normalize(ray_origin + ray_direction * outT - center);
  return true;
}

} // namespace math
} // namespace gero
