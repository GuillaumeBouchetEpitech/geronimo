
#include "headers.hpp"

TEST(physic_wrapper, query_shape_nothing) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  gero::physics::PhysicBodyDef bodyDef;
  bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef.shape.data.sphere.radius = 1.0f;
  bodyDef.mass = 0.0f;
  auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
  bodyRef->setPosition({0, 0, 0});
  world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

  world->step(0, 0, 0);

  gero::physics::PhysicShapeDef shapeDef;
  shapeDef.type = gero::physics::PhysicShapeDef::Type::sphere;
  shapeDef.data.sphere.radius = 1.0f;

  gero::physics::QueryShape::QueryShapeParams params(glm::vec3(1000, 0, 0), shapeDef);
  params.collisionGroup = -1;
  params.collisionMask = -1;
  gero::physics::QueryShape::QueryShapeParams::ResultArray<5> resultStack;
  world->getQueryShape().queryShape(params, resultStack);

  std::vector<gero::physics::AbstractPhysicBody*> resultHeap;
  world->getQueryShape().queryShape(params, resultHeap);

  ASSERT_EQ(resultStack.hasHit, false);
  ASSERT_EQ(resultStack.allBodiesTotal, 0);
  ASSERT_EQ(resultStack.allBodiesData.size(), 5);

  ASSERT_EQ(resultHeap.size(), 0);
}

TEST(physic_wrapper, query_shape_static_object) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  gero::physics::PhysicBodyDef bodyDef;
  bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef.shape.data.sphere.radius = 1.0f;
  bodyDef.mass = 0.0f;
  auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
  bodyRef->setPosition({0, 0, 0});
  world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

  world->step(0, 0, 0);

  gero::physics::PhysicShapeDef shapeDef;
  shapeDef.type = gero::physics::PhysicShapeDef::Type::sphere;
  shapeDef.data.sphere.radius = 5.0f;

  gero::physics::QueryShape::QueryShapeParams params(glm::vec3(5, 0, 0), shapeDef);
  params.collisionGroup = -1;
  params.collisionMask = -1;
  gero::physics::QueryShape::QueryShapeParams::ResultArray<5> resultStack;
  world->getQueryShape().queryShape(params, resultStack);

  std::vector<gero::physics::AbstractPhysicBody*> resultHeap;
  world->getQueryShape().queryShape(params, resultHeap);

  ASSERT_EQ(resultStack.hasHit, true);
  ASSERT_EQ(resultStack.allBodiesTotal, 1);
  ASSERT_EQ(resultStack.allBodiesData.size(), 5);
  ASSERT_EQ(resultStack.allBodiesData[0], bodyRef.get());

  ASSERT_EQ(resultHeap.size(), 1);
  ASSERT_EQ(resultHeap[0], bodyRef.get());
}

TEST(physic_wrapper, query_shape_all_static_objects) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  std::vector<gero::physics::BodyWeakRef> allBodyRef;
  allBodyRef.reserve(10);

  std::array<glm::vec3, 7> allBodyPos = {{
    glm::vec3(0, 0, 0),

    glm::vec3(+2, 0, 0),
    glm::vec3(0, +2, 0),
    glm::vec3(0, 0, +2),

    glm::vec3(-2, 0, 0),
    glm::vec3(0, -2, 0),
    glm::vec3(0, 0, -2),
  }};

  for (const glm::vec3 pos : allBodyPos) {
    gero::physics::PhysicBodyDef bodyDef;
    bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    bodyDef.shape.data.sphere.radius = 1.0f;
    bodyDef.mass = 0.0f;
    auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
    bodyRef->setPosition(pos);
    world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

    allBodyRef.push_back(bodyRef);
  }

  world->step(0, 0, 0);

  gero::physics::PhysicShapeDef shapeDef;
  shapeDef.type = gero::physics::PhysicShapeDef::Type::sphere;
  shapeDef.data.sphere.radius = 5.0f;

  gero::physics::QueryShape::QueryShapeParams params(glm::vec3(0, 0, 0), shapeDef);
  params.collisionGroup = -1;
  params.collisionMask = -1;
  gero::physics::QueryShape::QueryShapeParams::ResultArray<10> resultStack;
  world->getQueryShape().queryShape(params, resultStack);

  std::vector<gero::physics::AbstractPhysicBody*> resultHeap;
  world->getQueryShape().queryShape(params, resultHeap);

  ASSERT_EQ(resultStack.hasHit, true);
  ASSERT_EQ(resultStack.allBodiesTotal, 7);

  auto findBody = [&allBodyRef](const gero::physics::AbstractPhysicBody* inBody) {
    auto it = std::find_if(allBodyRef.begin(), allBodyRef.end(), [inBody](const gero::physics::BodyWeakRef& bodyRef) {
      return inBody == bodyRef.get();
    });
    return it != allBodyRef.end();
  };

  ASSERT_EQ(resultStack.allBodiesData.size(), 10);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(0)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(1)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(2)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(3)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(4)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(5)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(6)), true);

  ASSERT_EQ(resultHeap.size(), 7);
  ASSERT_EQ(findBody(resultHeap.at(0)), true);
  ASSERT_EQ(findBody(resultHeap.at(1)), true);
  ASSERT_EQ(findBody(resultHeap.at(2)), true);
  ASSERT_EQ(findBody(resultHeap.at(3)), true);
  ASSERT_EQ(findBody(resultHeap.at(4)), true);
  ASSERT_EQ(findBody(resultHeap.at(5)), true);
  ASSERT_EQ(findBody(resultHeap.at(6)), true);
}

TEST(physic_wrapper, query_shape_some_static_objects) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  std::vector<gero::physics::BodyWeakRef> allBodyRef;
  allBodyRef.reserve(10);

  std::array<glm::vec3, 7> allBodyPos = {{
    glm::vec3(0, 0, 0),

    glm::vec3(+2, 0, 0),
    glm::vec3(0, +2, 0),
    glm::vec3(0, 0, +2),

    glm::vec3(-2, 0, 0),
    glm::vec3(0, -2, 0),
    glm::vec3(0, 0, -2),
  }};

  for (const glm::vec3 pos : allBodyPos) {
    gero::physics::PhysicBodyDef bodyDef;
    bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    bodyDef.shape.data.sphere.radius = 1.0f;
    bodyDef.mass = 0.0f;
    auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
    bodyRef->setPosition(pos);
    world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

    allBodyRef.push_back(bodyRef);
  }

  world->step(0, 0, 0);

  gero::physics::PhysicShapeDef shapeDef;
  shapeDef.type = gero::physics::PhysicShapeDef::Type::sphere;
  shapeDef.data.sphere.radius = 2.0f;

  gero::physics::QueryShape::QueryShapeParams params(glm::vec3(2, 2, 0), shapeDef);
  params.collisionGroup = -1;
  params.collisionMask = -1;
  gero::physics::QueryShape::QueryShapeParams::ResultArray<10> resultStack;
  world->getQueryShape().queryShape(params, resultStack);

  std::vector<gero::physics::AbstractPhysicBody*> resultHeap;
  world->getQueryShape().queryShape(params, resultHeap);

  ASSERT_EQ(resultStack.hasHit, true);
  ASSERT_EQ(resultStack.allBodiesTotal, 3);

  auto findBody = [&allBodyRef](const gero::physics::AbstractPhysicBody* inBody) {
    auto it = std::find_if(allBodyRef.begin(), allBodyRef.end(), [inBody](const gero::physics::BodyWeakRef& bodyRef) {
      return inBody == bodyRef.get();
    });
    return it != allBodyRef.end();
  };

  ASSERT_EQ(resultStack.allBodiesData.size(), 10);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(0)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(1)), true);
  ASSERT_EQ(findBody(resultStack.allBodiesData.at(2)), true);

  ASSERT_EQ(resultHeap.size(), 3);
  ASSERT_EQ(findBody(resultHeap.at(0)), true);
  ASSERT_EQ(findBody(resultHeap.at(1)), true);
  ASSERT_EQ(findBody(resultHeap.at(2)), true);
}

TEST(physic_wrapper, query_shape_some_static_objects_with_collision_filters) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  const short filter1 = 0b0100;
  const short filter2 = 0b0010;

  struct TestDef {
    glm::vec3 pos;
    short filter;
  };

  const std::array<TestDef, 7> allBodyPos = {{
    {glm::vec3(0, 0, 0), filter1},

    {glm::vec3(+2, 0, 0), filter2},
    {glm::vec3(0, +2, 0), filter1},
    {glm::vec3(0, 0, +2), filter2},

    {glm::vec3(-2, 0, 0), filter2},
    {glm::vec3(0, -2, 0), filter1},
    {glm::vec3(0, 0, -2), filter2},
  }};

  const std::set<std::size_t> allExpectedIndices = {{0, 2, 5}};

  std::vector<gero::physics::BodyWeakRef> allBodyRef;
  allBodyRef.reserve(10);

  for (const TestDef& testDef : allBodyPos) {
    gero::physics::PhysicBodyDef bodyDef;
    bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    bodyDef.shape.data.sphere.radius = 1.0f;
    bodyDef.mass = 0.0f;
    auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
    bodyRef->setPosition(testDef.pos);
    world->getPhysicBodyManager().addBody(bodyRef, testDef.filter, testDef.filter);

    allBodyRef.push_back(bodyRef);
  }

  world->step(0, 0, 0);

  gero::physics::PhysicShapeDef shapeDef;
  shapeDef.type = gero::physics::PhysicShapeDef::Type::sphere;
  shapeDef.data.sphere.radius = 5.0f;

  gero::physics::QueryShape::QueryShapeParams params(glm::vec3(0, 0, 0), shapeDef);
  params.collisionGroup = filter1;
  params.collisionMask = filter1;
  gero::physics::QueryShape::QueryShapeParams::ResultArray<10> resultStack;
  world->getQueryShape().queryShape(params, resultStack);

  std::vector<gero::physics::AbstractPhysicBody*> resultHeap;
  world->getQueryShape().queryShape(params, resultHeap);

  ASSERT_EQ(resultStack.hasHit, true);
  ASSERT_EQ(resultStack.allBodiesTotal, 3);

  {
    ASSERT_EQ(resultStack.allBodiesData.size(), 10);

    int totalFound = 0;
    for (std::size_t ii = 0; ii < resultStack.allBodiesTotal; ++ii) {
      for (std::size_t index = 0; index < allBodyRef.size(); ++index) {
        if (resultStack.allBodiesData[ii] == allBodyRef[index].get() && allExpectedIndices.count(index) > 0) {
          ++totalFound;
        }
      }
    }

    ASSERT_EQ(totalFound, 3);
  }

  {
    ASSERT_EQ(resultHeap.size(), 3);

    int totalFound = 0;
    for (std::size_t ii = 0; ii < resultHeap.size(); ++ii) {
      for (std::size_t index = 0; index < allBodyRef.size(); ++index) {
        if (resultHeap[ii] == allBodyRef[index].get() && allExpectedIndices.count(index) > 0) {
          ++totalFound;
        }
      }
    }

    ASSERT_EQ(totalFound, 3);
  }
}

TEST(physic_wrapper, query_shape_some_static_objects_with_to_ignore) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  const std::array<glm::vec3, 7> allBodyPos = {{
    glm::vec3(0, 0, 0),

    glm::vec3(+2, 0, 0),
    glm::vec3(0, +2, 0),
    glm::vec3(0, 0, +2),

    glm::vec3(-2, 0, 0),
    glm::vec3(0, -2, 0),
    glm::vec3(0, 0, -2),
  }};

  const std::set<std::size_t> allExpectedIndices = {{0, 1, 2, 3, /*4,*/ 5, 6}};

  std::vector<gero::physics::BodyWeakRef> allBodyRef;
  allBodyRef.reserve(10);

  for (const glm::vec3& pos : allBodyPos) {
    gero::physics::PhysicBodyDef bodyDef;
    bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    bodyDef.shape.data.sphere.radius = 1.0f;
    bodyDef.mass = 0.0f;
    auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
    bodyRef->setPosition(pos);
    world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

    allBodyRef.push_back(bodyRef);
  }

  world->step(0, 0, 0);

  gero::physics::PhysicShapeDef shapeDef;
  shapeDef.type = gero::physics::PhysicShapeDef::Type::sphere;
  shapeDef.data.sphere.radius = 5.0f;

  gero::physics::QueryShape::QueryShapeParams params(glm::vec3(0, 0, 0), shapeDef);
  params.collisionGroup = -1;
  params.collisionMask = -1;
  params.toIgnore = &(*allBodyRef[4]);
  gero::physics::QueryShape::QueryShapeParams::ResultArray<10> resultStack;
  world->getQueryShape().queryShape(params, resultStack);

  std::vector<gero::physics::AbstractPhysicBody*> resultHeap;
  world->getQueryShape().queryShape(params, resultHeap);

  ASSERT_EQ(resultStack.hasHit, true);
  ASSERT_EQ(resultStack.allBodiesTotal, 6);

  {
    ASSERT_EQ(resultStack.allBodiesData.size(), 10);

    int totalFound = 0;
    for (std::size_t ii = 0; ii < resultStack.allBodiesTotal; ++ii) {
      for (std::size_t index = 0; index < allBodyRef.size(); ++index) {
        if (resultStack.allBodiesData[ii] == allBodyRef[index].get() && allExpectedIndices.count(index) > 0) {
          ++totalFound;
        }
      }
    }

    ASSERT_EQ(totalFound, 6);
  }

  {
    ASSERT_EQ(resultHeap.size(), 6);

    int totalFound = 0;
    for (std::size_t ii = 0; ii < resultHeap.size(); ++ii) {
      for (std::size_t index = 0; index < allBodyRef.size(); ++index) {
        if (resultHeap[ii] == allBodyRef[index].get() && allExpectedIndices.count(index) > 0) {
          ++totalFound;
        }
      }
    }

    ASSERT_EQ(totalFound, 6);
  }
}

#if 0

TEST(physic_wrapper, query_shape_static_object_out_of_scope) {
  auto world = gero::physics::AbstractPhysicWorld::create();

  gero::physics::PhysicBodyDef bodyDef;
  bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
  bodyDef.shape.data.sphere.radius = 1.0f;
  bodyDef.mass = 0.0f;
  auto bodyRef = world->getPhysicBodyManager().createBody(bodyDef);
  bodyRef->setPosition({0, 0, 0});
  world->getPhysicBodyManager().addBody(bodyRef, -1, -1);

  world->step(0, 0, 0);

  {
    gero::physics::QueryShape::QueryShapeParams params(glm::vec3(5, 0, 0), 5.0f);
    params.collisionGroup = -1;
    params.collisionMask = -1;
    gero::physics::QueryShape::QueryShapeParams::ResultArray<5> result;
    world->getQueryShape().queryShape(params, result);

    ASSERT_EQ(result.hasHit, true);
    ASSERT_EQ(result.allBodiesTotal, 1);
    ASSERT_EQ(result.allBodiesRefs.size(), 5);
    ASSERT_EQ(result.allBodiesRefs[0], bodyRef);
  }

  {
    gero::physics::QueryShape::QueryShapeParams params(glm::vec3(5, 0, 0), 5.0f);
    params.collisionGroup = -1;
    params.collisionMask = -1;
    gero::physics::QueryShape::QueryShapeParams::ResultArray<5> result;
    world->getQueryShape().queryShape(params, result);

    ASSERT_EQ(result.hasHit, true);
    ASSERT_EQ(result.allBodiesTotal, 1);
    ASSERT_EQ(result.allBodiesRefs.size(), 5);
    ASSERT_EQ(result.allBodiesRefs[0], bodyRef);
  }

  {
    gero::physics::QueryShape::QueryShapeParams params(glm::vec3(5, 0, 0), 5.0f);
    params.collisionGroup = -1;
    params.collisionMask = -1;
    gero::physics::QueryShape::QueryShapeParams::ResultArray<5> result;
    world->getQueryShape().queryShape(params, result);

    ASSERT_EQ(result.hasHit, true);
    ASSERT_EQ(result.allBodiesTotal, 1);
    ASSERT_EQ(result.allBodiesRefs.size(), 5);
    ASSERT_EQ(result.allBodiesRefs[0], bodyRef);
  }
}

#endif
