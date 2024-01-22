
#include "headers.hpp"

#include "geronimo/system/metrics/Clock.hpp"

TEST(physic_wrapper_perf, rayCast_closest_static_object) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  gero::physics::PhysicBodyDef bodyDef;
  bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef.shape.data.sphere.radius = 1.0f;
  bodyDef.mass = 0.0f;
  auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
  bodyRef->setPosition({0, 0, 0});
  world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

  world->step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::closest;
  paramsRay.toIgnore = nullptr;

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  // gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  std::vector<gero::physics::RayCaster::RayCastParams::ResultArray<5>> allResultsRayStack;
  allResultsRayStack.resize(1024 * 10);

  gero::metrics::Clock tmpClock;
  tmpClock.start();

  for (auto& currResult : allResultsRayStack) {
    world->getRayCaster().rayCast(paramsRay, currResult);
  }

  tmpClock.stop();

  D_MYLOG("duration: " << tmpClock.getDuration());

  // std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  // world->getRayCaster().rayCast(paramsRay, resultRayHeap);

  // gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  // world->getRayCaster().rayCast(paramsSphere, resultSphereStack);

  // std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  // world->getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  for (auto& currResult : allResultsRayStack) {
    ASSERT_EQ(currResult.hasHit, true);
    ASSERT_EQ(currResult.allImpactsTotal, 1);
    ASSERT_EQ(currResult.allImpactsData.size(), 5);
    ASSERT_EQ(currResult.allImpactsData[0].body, bodyRef.get());
    ASSERT_NEAR(currResult.allImpactsData[0].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(currResult.allImpactsData[0].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(currResult.allImpactsData[0].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(currResult.allImpactsData[0].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(currResult.allImpactsData[0].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(currResult.allImpactsData[0].impactPoint.z, 1, 0.001);
  }

  // ASSERT_EQ(resultRayHeap.size(), 1);
  // ASSERT_EQ(resultRayHeap[0].body, bodyRef.get());
  // ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
  // ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
  // ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
  // ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
  // ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
  // ASSERT_NEAR(resultRayHeap[0].impactPoint.z, 1, 0.001);

  // ASSERT_EQ(resultSphereStack.hasHit, true);
  // ASSERT_EQ(resultSphereStack.allImpactsTotal, 1);
  // ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
  // ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef.get());
  // ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  // ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  // ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  // ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  // ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  // ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  // ASSERT_EQ(resultSphereHeap.size(), 1);
  // ASSERT_EQ(resultSphereHeap[0].body, bodyRef.get());
  // ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
  // ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
  // ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
  // ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
  // ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
  // ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, 1, 0.001);
}
