
#include "rayIntersectTriangle.hpp"

#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace math {

bool intersectTriangle(const glm::vec3& ray_origin,
                       const glm::vec3& ray_direction,
                       const glm::vec3& v0,
                       const glm::vec3& v1,
                       const glm::vec3& v2,
                       RayCastResult& outData) {
  // float nearValue = 0.001; // TODO: hardcoded
  // float farValue = 100.0; // TODO: hardcoded

  const glm::vec3 v1v0 = v1 - v0;
  const glm::vec3 v2v0 = v2 - v0;
  const glm::vec3 rov0 = ray_origin - v0;

  const glm::vec3 n = glm::cross(v1v0, v2v0);

  // D_MYLOG("v1v0 -> " << v1v0);
  // D_MYLOG("v2v0 -> " << v2v0);
  // D_MYLOG("rov0 -> " << rov0);
  // D_MYLOG("n    -> " << n);

  const float nDotRayDirection = glm::dot(ray_direction, n);
  // D_MYLOG("nDotRayDirection -> " << nDotRayDirection);

  if (std::abs(nDotRayDirection) < 0.0001f) { // Almost 0
    // D_MYLOG("fail 1");
    return false; // They are parallel, so they don't intersect!
  }

  const glm::vec3 q = glm::cross(rov0, ray_direction);
  const float d = 1.0f / nDotRayDirection;
  const float u = d * glm::dot(-q, v2v0);
  const float v = d * glm::dot(q, v1v0);
  const float t = d * glm::dot(-n, rov0);

  // console.log(glm.vec3.dot(ray_direction, n));
  // console.log(ray_direction);
  // console.log(n);
  // console.log({d});
  // console.log({t});

  // if (u < 0.0 || v < 0.0 || (u + v) > 1.0 || t < nearValue || t > farValue) {
  //   return false;
  // }
  if (u < 0.0 || v < 0.0 || (u + v) > 1.0 /*|| t < 0 || t > 1*/) {
    // outData.distance = -1;
    // outData.normal = glm::vec3(0,0,0);
    // D_MYLOG("fail 2");
    return false;
  }

  // const length = glm.vec3.length(normal);

  outData.distance = t;

  outData.normal = -n;
  // outData.normal = n;

  float length = glm::length(outData.normal);
  // safe normalize -> check the length first to avoid dividing by zero
  if (length > 0) {
    outData.normal /= length; // normalize
  }

  // outDistance = t;
  return true;
}

} // namespace math
} // namespace gero
