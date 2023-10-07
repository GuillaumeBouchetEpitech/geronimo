
#include "headers.hpp"

TEST(physic_wrapper, rayCast_nothing) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef;
  bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef.shape.data.sphere.radius = 1.0f;
  bodyDef.mass = 0.0f;
  auto bodyRef = world.getPhysicBodyManager().createBody(bodyDef);
  bodyRef->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef, -1, -1);

  world.step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(1000, 0, 10), glm::vec3(1000, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::closest;
  paramsRay.toIgnore = nullptr;

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  world.getRayCaster().rayCast(paramsRay, resultRayStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  world.getRayCaster().rayCast(paramsRay, resultRayHeap);

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  ASSERT_EQ(resultRayStack.hasHit, false);
  ASSERT_EQ(resultRayStack.allImpactsTotal, 0);
  ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);

  ASSERT_EQ(resultRayHeap.size(), 0);

  ASSERT_EQ(resultSphereStack.hasHit, false);
  ASSERT_EQ(resultSphereStack.allImpactsTotal, 0);
  ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);

  ASSERT_EQ(resultSphereHeap.size(), 0);
}

TEST(physic_wrapper, rayCast_closest_static_object) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef;
  bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef.shape.data.sphere.radius = 1.0f;
  bodyDef.mass = 0.0f;
  auto bodyRef = world.getPhysicBodyManager().createBody(bodyDef);
  bodyRef->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef, -1, -1);

  world.step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::closest;
  paramsRay.toIgnore = nullptr;

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  world.getRayCaster().rayCast(paramsRay, resultRayStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  world.getRayCaster().rayCast(paramsRay, resultRayHeap);

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  ASSERT_EQ(resultRayStack.hasHit, true);
  ASSERT_EQ(resultRayStack.allImpactsTotal, 1);
  ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultRayStack.allImpactsData[0].body, bodyRef.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultRayHeap.size(), 1);
  ASSERT_EQ(resultRayHeap[0].body, bodyRef.get());
  ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultSphereStack.hasHit, true);
  ASSERT_EQ(resultSphereStack.allImpactsTotal, 1);
  ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultSphereHeap.size(), 1);
  ASSERT_EQ(resultSphereHeap[0].body, bodyRef.get());
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, 1, 0.001);
}

TEST(physic_wrapper, rayCast_closest_static_object_with_collision_filters) {
  gero::physics::PhysicWorld world;

  const short filter1 = 0b0100;
  const short filter2 = 0b0010;

  gero::physics::PhysicBodyDef bodyDef_1;
  bodyDef_1.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_1.shape.data.sphere.radius = 1.0f;
  bodyDef_1.mass = 0.0f;
  auto bodyRef_1 = world.getPhysicBodyManager().createBody(bodyDef_1);
  bodyRef_1->setPosition({0, 0, 5});
  world.getPhysicBodyManager().addBody(bodyRef_1, filter1, filter1);

  gero::physics::PhysicBodyDef bodyDef_2;
  bodyDef_2.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_2.shape.data.sphere.radius = 1.0f;
  bodyDef_2.mass = 0.0f;
  auto bodyRef_2 = world.getPhysicBodyManager().createBody(bodyDef_2);
  bodyRef_2->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef_2, filter2, filter2);

  world.step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = filter2;
  paramsRay.collisionMask = filter2;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::closest;
  paramsRay.toIgnore = nullptr;

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  world.getRayCaster().rayCast(paramsRay, resultRayStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  world.getRayCaster().rayCast(paramsRay, resultRayHeap);

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  ASSERT_EQ(resultRayStack.hasHit, true);
  ASSERT_EQ(resultRayStack.allImpactsTotal, 1);
  ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultRayStack.allImpactsData[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultRayHeap.size(), 1);
  ASSERT_EQ(resultRayHeap[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultSphereStack.hasHit, true);
  ASSERT_EQ(resultSphereStack.allImpactsTotal, 1);
  ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultSphereHeap.size(), 1);
  ASSERT_EQ(resultSphereHeap[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, 1, 0.001);
}

TEST(physic_wrapper, rayCast_closest_static_object_with_ignore_parameter) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef_1;
  bodyDef_1.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_1.shape.data.sphere.radius = 1.0f;
  bodyDef_1.mass = 0.0f;
  auto bodyRef_1 = world.getPhysicBodyManager().createBody(bodyDef_1);
  bodyRef_1->setPosition({0, 0, 5});
  world.getPhysicBodyManager().addBody(bodyRef_1, -1, -1);

  gero::physics::PhysicBodyDef bodyDef_2;
  bodyDef_2.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_2.shape.data.sphere.radius = 1.0f;
  bodyDef_2.mass = 0.0f;
  auto bodyRef_2 = world.getPhysicBodyManager().createBody(bodyDef_2);
  bodyRef_2->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef_2, -1, -1);

  world.step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::closest;
  paramsRay.toIgnore = &(*bodyRef_1);

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  world.getRayCaster().rayCast(paramsRay, resultRayStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  world.getRayCaster().rayCast(paramsRay, resultRayHeap);

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  ASSERT_EQ(resultRayStack.hasHit, true);
  ASSERT_EQ(resultRayStack.allImpactsTotal, 1);
  ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultRayStack.allImpactsData[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultRayHeap.size(), 1);
  ASSERT_EQ(resultRayHeap[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultSphereStack.hasHit, true);
  ASSERT_EQ(resultSphereStack.allImpactsTotal, 1);
  ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, 1, 0.001);

  ASSERT_EQ(resultSphereHeap.size(), 1);
  ASSERT_EQ(resultSphereHeap[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, 1, 0.001);
}

TEST(physic_wrapper, rayCast_every_static_objects) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef_1;
  bodyDef_1.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_1.shape.data.sphere.radius = 1.0f;
  bodyDef_1.mass = 0.0f;
  auto bodyRef_1 = world.getPhysicBodyManager().createBody(bodyDef_1);
  bodyRef_1->setPosition({0, 0, 5});
  world.getPhysicBodyManager().addBody(bodyRef_1, -1, -1);

  gero::physics::PhysicBodyDef bodyDef_2;
  bodyDef_2.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_2.shape.data.sphere.radius = 1.0f;
  bodyDef_2.mass = 0.0f;
  auto bodyRef_2 = world.getPhysicBodyManager().createBody(bodyDef_2);
  bodyRef_2->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef_2, -1, -1);

  world.step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::everything;
  // paramsRay.toIgnore = &(*bodyRef_1);

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  world.getRayCaster().rayCast(paramsRay, resultRayStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  world.getRayCaster().rayCast(paramsRay, resultRayHeap);

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  ASSERT_EQ(resultRayStack.hasHit, true);
  ASSERT_EQ(resultRayStack.allImpactsTotal, 2);
  ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultRayStack.allImpactsData[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.z, 1, 0.001);
  ASSERT_EQ(resultRayStack.allImpactsData[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.z, 6, 0.001);

  ASSERT_EQ(resultRayHeap.size(), 2);
  ASSERT_EQ(resultRayHeap[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.z, 1, 0.001);
  ASSERT_EQ(resultRayHeap[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultRayHeap[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactPoint.z, 6, 0.001);

  ASSERT_EQ(resultSphereStack.hasHit, true);
  ASSERT_EQ(resultSphereStack.allImpactsTotal, 2);
  ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, 1, 0.001);
  ASSERT_EQ(resultSphereStack.allImpactsData[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.z, 6, 0.001);

  ASSERT_EQ(resultSphereHeap.size(), 2);
  ASSERT_EQ(resultSphereHeap[0].body, bodyRef_2.get());
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, 1, 0.001);
  ASSERT_EQ(resultSphereHeap[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultSphereHeap[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactPoint.z, 6, 0.001);
}

TEST(physic_wrapper, rayCast_every_static_objects_with_ignore_parameter) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef_1;
  bodyDef_1.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_1.shape.data.sphere.radius = 1.0f;
  bodyDef_1.mass = 0.0f;
  auto bodyRef_1 = world.getPhysicBodyManager().createBody(bodyDef_1);
  bodyRef_1->setPosition({0, 0, 5});
  world.getPhysicBodyManager().addBody(bodyRef_1, -1, -1);

  gero::physics::PhysicBodyDef bodyDef_2;
  bodyDef_2.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_2.shape.data.sphere.radius = 1.0f;
  bodyDef_2.mass = 0.0f;
  auto bodyRef_2 = world.getPhysicBodyManager().createBody(bodyDef_2);
  bodyRef_2->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef_2, -1, -1);

  gero::physics::PhysicBodyDef bodyDef_3;
  bodyDef_3.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_3.shape.data.sphere.radius = 1.0f;
  bodyDef_3.mass = 0.0f;
  auto bodyRef_3 = world.getPhysicBodyManager().createBody(bodyDef_3);
  bodyRef_3->setPosition({0, 0, -5});
  world.getPhysicBodyManager().addBody(bodyRef_3, -1, -1);

  world.step(0, 0, 0);

  gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::everything;
  paramsRay.toIgnore = &(*bodyRef_2);

  gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
  world.getRayCaster().rayCast(paramsRay, resultRayStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
  world.getRayCaster().rayCast(paramsRay, resultRayHeap);

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
  world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

  std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
  world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

  ASSERT_EQ(resultRayStack.hasHit, true);
  ASSERT_EQ(resultRayStack.allImpactsTotal, 2);
  ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultRayStack.allImpactsData[0].body, bodyRef_3.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.z, -4, 0.001);
  ASSERT_EQ(resultRayStack.allImpactsData[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.z, 6, 0.001);

  ASSERT_EQ(resultRayHeap.size(), 2);
  ASSERT_EQ(resultRayHeap[0].body, bodyRef_3.get());
  ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[0].impactPoint.z, -4, 0.001);
  ASSERT_EQ(resultRayHeap[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultRayHeap[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultRayHeap[1].impactPoint.z, 6, 0.001);

  ASSERT_EQ(resultSphereStack.hasHit, true);
  ASSERT_EQ(resultSphereStack.allImpactsTotal, 2);
  ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
  ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef_3.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, -4, 0.001);
  ASSERT_EQ(resultSphereStack.allImpactsData[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.z, 6, 0.001);

  ASSERT_EQ(resultSphereHeap.size(), 2);
  ASSERT_EQ(resultSphereHeap[0].body, bodyRef_3.get());
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, -4, 0.001);
  ASSERT_EQ(resultSphereHeap[1].body, bodyRef_1.get());
  ASSERT_NEAR(resultSphereHeap[1].impactNormal.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactNormal.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactNormal.z, 1, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactPoint.x, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactPoint.y, 0, 0.001f);
  ASSERT_NEAR(resultSphereHeap[1].impactPoint.z, 6, 0.001);
}

TEST(physic_wrapper, rayCast_every_static_objects_being_looped) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef_1;
  bodyDef_1.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_1.shape.data.sphere.radius = 1.0f;
  bodyDef_1.mass = 0.0f;
  auto bodyRef_1 = world.getPhysicBodyManager().createBody(bodyDef_1);
  bodyRef_1->setPosition({0, 0, 5});
  world.getPhysicBodyManager().addBody(bodyRef_1, -1, -1);

  gero::physics::PhysicBodyDef bodyDef_2;
  bodyDef_2.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_2.shape.data.sphere.radius = 1.0f;
  bodyDef_2.mass = 0.0f;
  auto bodyRef_2 = world.getPhysicBodyManager().createBody(bodyDef_2);
  bodyRef_2->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef_2, -1, -1);

  gero::physics::PhysicBodyDef bodyDef_3;
  bodyDef_3.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef_3.shape.data.sphere.radius = 1.0f;
  bodyDef_3.mass = 0.0f;
  auto bodyRef_3 = world.getPhysicBodyManager().createBody(bodyDef_3);
  bodyRef_3->setPosition({0, 0, -5});
  world.getPhysicBodyManager().addBody(bodyRef_3, -1, -1);

  world.step(0, 0, 0);

  for (int ii = 0; ii < 10; ++ii) {

    gero::physics::RayCaster::RayCastParams paramsRay(glm::vec3(0, 0, 10), glm::vec3(0, 0, -10));
    paramsRay.radius = 0.0f;
    paramsRay.collisionGroup = -1;
    paramsRay.collisionMask = -1;
    paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::everything;
    paramsRay.toIgnore = &(*bodyRef_2);

    gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
    paramsSphere.radius = 1.0f;

    gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRayStack;
    world.getRayCaster().rayCast(paramsRay, resultRayStack);

    std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultRayHeap;
    world.getRayCaster().rayCast(paramsRay, resultRayHeap);

    gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphereStack;
    world.getRayCaster().rayCast(paramsSphere, resultSphereStack);

    std::vector<gero::physics::RayCaster::RayCastParams::ResultImpact> resultSphereHeap;
    world.getRayCaster().rayCast(paramsSphere, resultSphereHeap);

    ASSERT_EQ(resultRayStack.hasHit, true);
    ASSERT_EQ(resultRayStack.allImpactsTotal, 2);
    ASSERT_EQ(resultRayStack.allImpactsData.size(), 5);
    ASSERT_EQ(resultRayStack.allImpactsData[0].body, bodyRef_3.get());
    ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[0].impactPoint.z, -4, 0.001);
    ASSERT_EQ(resultRayStack.allImpactsData[1].body, bodyRef_1.get());
    ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[1].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultRayStack.allImpactsData[1].impactPoint.z, 6, 0.001);

    ASSERT_EQ(resultRayHeap.size(), 2);
    ASSERT_EQ(resultRayHeap[0].body, bodyRef_3.get());
    ASSERT_NEAR(resultRayHeap[0].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[0].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[0].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultRayHeap[0].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[0].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[0].impactPoint.z, -4, 0.001);
    ASSERT_EQ(resultRayHeap[1].body, bodyRef_1.get());
    ASSERT_NEAR(resultRayHeap[1].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[1].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[1].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultRayHeap[1].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[1].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultRayHeap[1].impactPoint.z, 6, 0.001);

    ASSERT_EQ(resultSphereStack.hasHit, true);
    ASSERT_EQ(resultSphereStack.allImpactsTotal, 2);
    ASSERT_EQ(resultSphereStack.allImpactsData.size(), 5);
    ASSERT_EQ(resultSphereStack.allImpactsData[0].body, bodyRef_3.get());
    ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[0].impactPoint.z, -4, 0.001);
    ASSERT_EQ(resultSphereStack.allImpactsData[1].body, bodyRef_1.get());
    ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereStack.allImpactsData[1].impactPoint.z, 6, 0.001);

    ASSERT_EQ(resultSphereHeap.size(), 2);
    ASSERT_EQ(resultSphereHeap[0].body, bodyRef_3.get());
    ASSERT_NEAR(resultSphereHeap[0].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[0].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[0].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultSphereHeap[0].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[0].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[0].impactPoint.z, -4, 0.001);
    ASSERT_EQ(resultSphereHeap[1].body, bodyRef_1.get());
    ASSERT_NEAR(resultSphereHeap[1].impactNormal.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[1].impactNormal.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[1].impactNormal.z, 1, 0.001f);
    ASSERT_NEAR(resultSphereHeap[1].impactPoint.x, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[1].impactPoint.y, 0, 0.001f);
    ASSERT_NEAR(resultSphereHeap[1].impactPoint.z, 6, 0.001);
  }
}

TEST(physic_wrapper, rayCast_repro_static_box_bug) {
  gero::physics::PhysicWorld world;

  gero::physics::PhysicBodyDef bodyDef_1;
  bodyDef_1.shape.type = gero::physics::PhysicShapeDef::Type::box;
  bodyDef_1.shape.data.box.size = {1, 1, 1};
  bodyDef_1.mass = 0.0f;
  auto bodyRef_1 = world.getPhysicBodyManager().createBody(bodyDef_1);
  bodyRef_1->setPosition({0, 0, 0});
  world.getPhysicBodyManager().addBody(bodyRef_1, -1, -1);

  // gero::physics::PhysicBodyDef bodyDef_2;
  // bodyDef_2.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  // bodyDef_2.shape.data.sphere.radius = 1.0f;
  // bodyDef_2.mass = 0.0f;
  // auto bodyRef_2 = world.getPhysicBodyManager().createBody(bodyDef_2);
  // bodyRef_2->setPosition({0, 0, 0});
  // world.getPhysicBodyManager().addBody(bodyRef_2, -1, -1);

  // gero::physics::PhysicBodyDef bodyDef_3;
  // bodyDef_3.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  // bodyDef_3.shape.data.sphere.radius = 1.0f;
  // bodyDef_3.mass = 0.0f;
  // auto bodyRef_3 = world.getPhysicBodyManager().createBody(bodyDef_3);
  // bodyRef_3->setPosition({0, 0, -5});
  // world.getPhysicBodyManager().addBody(bodyRef_3, -1, -1);

  world.step(0, 0, 0);

  const glm::vec3 rayFrom = glm::vec3(10.5f, 10.0f, 0);
  const glm::vec3 rayDir = -glm::vec3(20.0f, 20.0f, 0);
  const glm::vec3 rayTo = rayFrom + rayDir;
  // const float angle = std::atan2(rayDir.y, rayDir.x);

  gero::physics::RayCaster::RayCastParams paramsRay(rayFrom, rayTo);
  paramsRay.radius = 0.0f;
  paramsRay.collisionGroup = -1;
  paramsRay.collisionMask = -1;
  paramsRay.type = gero::physics::RayCaster::RayCastParams::Type::closest;
  // paramsRay.toIgnore = &(*bodyRef_2);

  // gero::physics::RayCaster::RayCastParams paramsSphere = paramsRay;
  // paramsSphere.radius = 1.0f;

  gero::physics::RayCaster::RayCastParams::ResultArray<5> resultRay;
  world.getRayCaster().rayCast(paramsRay, resultRay);

  // gero::physics::RayCaster::RayCastParams::ResultArray<5> resultSphere;
  // world.getRayCaster().rayCast(paramsSphere, resultSphere);

  ASSERT_EQ(resultRay.hasHit, true);
  ASSERT_EQ(resultRay.allImpactsTotal, 1);
  ASSERT_EQ(resultRay.allImpactsData.size(), 5);
  ASSERT_EQ(resultRay.allImpactsData[0].body, bodyRef_1.get());
  ASSERT_NEAR(resultRay.allImpactsData[0].impactNormal.x, 1.0f, 0.1f);
  ASSERT_NEAR(resultRay.allImpactsData[0].impactNormal.y, 0.0f, 0.1f);
  ASSERT_NEAR(resultRay.allImpactsData[0].impactNormal.z, 0, 0.1f);
  ASSERT_NEAR(resultRay.allImpactsData[0].impactPoint.x, 0.5f, 0.1f);
  ASSERT_NEAR(resultRay.allImpactsData[0].impactPoint.y, 0.0f, 0.1f);
  ASSERT_NEAR(resultRay.allImpactsData[0].impactPoint.z, 0, 0.1f);

  // ASSERT_EQ(resultSphere.hasHit, true);
  // ASSERT_EQ(resultSphere.allImpactsTotal, 1);
  // ASSERT_EQ(resultSphere.allImpactsData.size(), 5);
  // ASSERT_EQ(resultSphere.allImpactsData[0].body, bodyRef_1.get());
  // // ASSERT_NEAR(resultSphere.allImpactsData[0].impactNormal.x,
  // std::cos(angle), 0.1f);
  // // ASSERT_NEAR(resultSphere.allImpactsData[0].impactNormal.y,
  // std::sin(angle), 0.1f);
  // ASSERT_NEAR(resultSphere.allImpactsData[0].impactNormal.x, 1.0f, 0.1f);
  // ASSERT_NEAR(resultSphere.allImpactsData[0].impactNormal.y, 0.4f, 0.1f);
  // ASSERT_NEAR(resultSphere.allImpactsData[0].impactNormal.z, 0, 0.1f);
  // ASSERT_NEAR(resultSphere.allImpactsData[0].impactPoint.x, 0.5f, 0.1f);
  // ASSERT_NEAR(resultSphere.allImpactsData[0].impactPoint.y, 0.5f, 0.1f);
  // ASSERT_NEAR(resultSphere.allImpactsData[0].impactPoint.z, 0, 0.1f);
}
