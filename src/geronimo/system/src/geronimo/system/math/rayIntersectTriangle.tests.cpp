
#include "./rayIntersectTriangle.hpp"

#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/constants.hpp"

#include <memory>

#include "gtest/gtest.h"

TEST(system_math, ray_intersect_triangle) {

  gero::math::RayCastResult tmpResult;

  glm::mat4 transform = glm::identity<glm::mat4>();
  transform *= glm::translate(transform, glm::vec3(100, 100, 100));
  transform *= glm::mat4_cast(glm::angleAxis(gero::math::qpi, glm::vec3(0, 0, 1)));
  // transform *= glm::translate(transform,  glm::vec3(100,100,100));

  const glm::vec4 rayOrigin = glm::vec4(-5, 5, 5, 1);
  const glm::vec4 rayDirection = glm::vec4(1, 0, 0, 0);
  const glm::vec4 v0 = glm::vec4(0, 0, 0, 1);
  const glm::vec4 v1 = glm::vec4(0, 30, 0, 1);
  const glm::vec4 v2 = glm::vec4(0, 0, 30, 1);

  constexpr float expectedDistance = 5.0f;
  const glm::vec4 expectedNormal = glm::vec4(-1, 0, 0, 0);

  bool hasHit = gero::math::intersectTriangle(
    transform * rayOrigin, transform * rayDirection, transform * v0, transform * v1, transform * v2, tmpResult);

  D_MYLOG("hasHit -> " << hasHit);
  D_MYLOG("tmpResult.distance -> " << tmpResult.distance);
  D_MYLOG("tmpResult.normal -> " << tmpResult.normal);

  const glm::vec4 tmpExpectedNormal = transform * expectedNormal;

  ASSERT_EQ(hasHit, true);
  ASSERT_FLOAT_EQ(tmpResult.distance, expectedDistance);
  ASSERT_FLOAT_EQ(tmpResult.normal.x, tmpExpectedNormal.x);
  ASSERT_FLOAT_EQ(tmpResult.normal.y, tmpExpectedNormal.y);
  ASSERT_FLOAT_EQ(tmpResult.normal.z, tmpExpectedNormal.z);
}
