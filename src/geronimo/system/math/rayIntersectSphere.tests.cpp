
#include "./rayIntersectSphere.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <memory>

#include "gtest/gtest.h"

TEST(system_math, ray_intersect_sphere) {

  gero::math::RayCastResult tmpResult;

  // const glm::vec3 offsetPos = glm::vec3(-10,-10,-10);
  const glm::vec3 offsetPos = glm::vec3(0,0,0);

  bool hasHit = gero::math::intersectSphere(
    glm::vec3(-20,0,0) + offsetPos,
    glm::vec3(1,0,0),
    glm::vec3(0,0,0) + offsetPos,
    10.0f,
    tmpResult
  );

  D_MYLOG("hasHit -> " << hasHit);
  D_MYLOG("tmpResult.distance -> " << tmpResult.distance);
  D_MYLOG("tmpResult.normal -> " << tmpResult.normal);

  ASSERT_EQ(hasHit, true);
  ASSERT_FLOAT_EQ(tmpResult.distance, 10.0f);
  ASSERT_FLOAT_EQ(tmpResult.normal.x, -1);
  ASSERT_FLOAT_EQ(tmpResult.normal.y, 0);
  ASSERT_FLOAT_EQ(tmpResult.normal.z, 0);
}
