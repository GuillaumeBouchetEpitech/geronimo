
#include "Context.hpp"

#include "application/defines.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/physics/body/PhysicBodyDef.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/constants.hpp"

void Context::initializePhysicResources() {

  {
    logic.heightField.generate(32, 1.5f, 20.0f);

    const uint32_t fieldSize = logic.heightField.getFieldSize();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<int32_t> indices;

    {
      vertices.reserve(2048 * 4);
      indices.reserve(2048 * 6);

      for (uint32_t yy = 0; yy + 1 < fieldSize; ++yy)
        for (uint32_t xx = 0; xx + 1 < fieldSize; ++xx) {
          const int32_t index = int32_t(vertices.size());

          vertices.push_back(logic.heightField.getPosition(xx + 0, yy + 0));
          vertices.push_back(logic.heightField.getPosition(xx + 1, yy + 0));
          vertices.push_back(logic.heightField.getPosition(xx + 1, yy + 1));
          vertices.push_back(logic.heightField.getPosition(xx + 0, yy + 1));

          normals.push_back(logic.heightField.getNormal(xx + 0, yy + 0));
          normals.push_back(logic.heightField.getNormal(xx + 1, yy + 0));
          normals.push_back(logic.heightField.getNormal(xx + 1, yy + 1));
          normals.push_back(logic.heightField.getNormal(xx + 0, yy + 1));

          //

          indices.push_back(index + 1);
          indices.push_back(index + 2);
          indices.push_back(index + 3);

          indices.push_back(index + 1);
          indices.push_back(index + 3);
          indices.push_back(index + 0);
        }
    }

    const float arenaHSize = logic.heightField.getArenaSize() * 0.5f;

    {
      gero::physics::PhysicBodyDef def;

      def.shape.type = gero::physics::PhysicShapeDef::Type::staticMesh;
      def.shape.data.staticMesh.verticesData = vertices.data();
      def.shape.data.staticMesh.verticesLength = vertices.size();
      def.shape.data.staticMesh.indicesData = indices.data();
      def.shape.data.staticMesh.indicesLength = indices.size();

      def.mass = 0.0f;
      def.group = gero::asValue(Groups::ground);
      def.mask = gero::asValue(Masks::ground);

      auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

      body->setPosition({-arenaHSize, -arenaHSize, 0});
      body->setFriction(0.5f);
      body->setUserValue(0);
    }

    {

      const glm::vec3 offset = glm::vec3(-arenaHSize, -arenaHSize, 0.0f);

      gero::graphics::MakeGeometries::Vertices geoVertices;
      vertices.reserve(1024 * 8);

      for (std::size_t ii = 0; ii < indices.size(); ii += 3) {
        const int32_t index1 = indices.at(ii + 0);
        const int32_t index2 = indices.at(ii + 1);
        const int32_t index3 = indices.at(ii + 2);

        const glm::vec3 pos1 = vertices.at(std::size_t(index1)) + offset;
        const glm::vec3 pos2 = vertices.at(std::size_t(index2)) + offset;
        const glm::vec3 pos3 = vertices.at(std::size_t(index3)) + offset;

        const glm::vec3 normal1 = normals.at(std::size_t(index1));
        const glm::vec3 normal2 = normals.at(std::size_t(index2));
        const glm::vec3 normal3 = normals.at(std::size_t(index3));

        geoVertices.push_back({pos1, normal1});
        geoVertices.push_back({pos2, normal2});
        geoVertices.push_back({pos3, normal3});
      }

      // gero::graphics::MakeGeometries::convertToPerFacesNormals(geoVertices);

      graphic.scene.geometriesStackRenderer.createAlias(100, geoVertices);
    }
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::box;
    def.shape.data.box.size = {75, 75, 1};
    def.mass = 0.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({0, 0, -5});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    def.shape.data.sphere.radius = 1.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({1, 1, 10});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    def.shape.data.sphere.radius = 2.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({2, 2, 10});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::box;
    def.shape.data.box.size = {4, 2, 1};
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({3, 3, 5});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::capsule;
    def.shape.data.capsule.radius = 0.5f;
    def.shape.data.capsule.height = 2.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({4, 4, 5});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::capsule;
    def.shape.data.capsule.radius = 1.0f;
    def.shape.data.capsule.height = 4.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({5, 5, 5});
  }

  {

    gero::physics::PhysicBodyDef defA;
    defA.shape.type = gero::physics::PhysicShapeDef::Type::box;
    defA.shape.data.box.size = {1, 1, 2};
    defA.mass = 0.0f;
    auto bodyA = this->physic.world->getPhysicBodyManager().createAndAddBody(defA);
    bodyA->setPosition({5, 5, 7});

    gero::physics::PhysicBodyDef defB;
    defB.shape.type = gero::physics::PhysicShapeDef::Type::box;
    defB.shape.data.box.size = {3, 1, 1};
    defB.mass = 1.0f;
    auto bodyB = this->physic.world->getPhysicBodyManager().createAndAddBody(defB);
    bodyB->setPosition({5, 5, 6});

    gero::physics::PhysicHingeConstraintDef constraintDef;
    constraintDef.body_a = bodyA;
    constraintDef.coord_a = glm::vec3(0, 0, -1);
    constraintDef.body_b = bodyB;
    constraintDef.coord_b = glm::vec3(1, 1, 0);
    constraintDef.axis = glm::vec3(0, 1, 0);

    this->physic.world->getPhysicHingeConstraintManager().createAndAdd(constraintDef);
  }

  // {

  //   const glm::vec3 k_anchor = glm::vec3(5,15,10);

  //   gero::physics::PhysicBodyDef defA;
  //   defA.shape.type = gero::physics::PhysicShapeDef::Type::box;
  //   defA.shape.data.box.size = {1, 1, 1};
  //   defA.mass = 0.0f;
  //   auto bodyA = this->physic.world->getPhysicBodyManager().createAndAddBody(defA);
  //   bodyA->setPosition(k_anchor);

  //   gero::physics::PhysicBodyDef defB;
  //   defB.shape.type = gero::physics::PhysicShapeDef::Type::box;
  //   defB.shape.data.box.size = {3, 2, 1};
  //   defB.mass = 1.0f;
  //   auto bodyB = this->physic.world->getPhysicBodyManager().createAndAddBody(defB);
  //   bodyB->setPosition(k_anchor + glm::vec3(3,0,0));

  //   gero::physics::PhysicUniversalConstraintDef constraintDef;
  //   constraintDef.body_a = bodyA;
  //   constraintDef.body_b = bodyB;
  //   constraintDef.anchor = k_anchor;
  //   constraintDef.axis1 = glm::vec3(0,0,1);
  //   constraintDef.axis2 = glm::vec3(0,1,0);

  //   auto constraintRef = this->physic.world->getPhysicUniversalConstraintManager().createAndAdd(constraintDef);

  //   constraintRef->setLowerLimit(gero::math::pi * -0.6f, gero::math::hpi * -0.5f);
  //   constraintRef->setUpperLimit(gero::math::hpi * +0.1f, gero::math::hpi * +0.1f);

  // }

  //   {

  //     const glm::vec3 k_anchor = glm::vec3(5,-15,10);

  //     gero::physics::PhysicBodyDef defA;
  //     defA.shape.type = gero::physics::PhysicShapeDef::Type::box;
  //     defA.shape.data.box.size = {1, 1, 1};
  //     defA.mass = 0.0f;
  //     auto bodyA = this->physic.world->getPhysicBodyManager().createAndAddBody(defA);
  //     bodyA->setPosition(k_anchor + glm::vec3(0,0,0));

  //     gero::physics::PhysicBodyDef defB;
  //     defB.shape.type = gero::physics::PhysicShapeDef::Type::box;
  //     defB.shape.data.box.size = {3, 2, 1};
  //     defB.mass = 1.0f;
  //     auto bodyB = this->physic.world->getPhysicBodyManager().createAndAddBody(defB);
  //     bodyB->setPosition(k_anchor + glm::vec3(3,0,0));
  //     bodyB->applyCentralImpulse(glm::vec3(1,1,1));

  // #if 1

  //     gero::physics::PhysicConeTwistConstraintDef constraintDef;
  //     constraintDef.body_a = bodyA;
  //     constraintDef.body_b = bodyB;
  //     constraintDef.center = k_anchor;
  //     // constraintDef.center = glm::vec3(0,0,0);

  //     auto constraintRef = this->physic.world->getPhysicConeTwistConstraintManager().createAndAdd(constraintDef);

  //     constraintRef->setLimit(0.0f, 0.0f, 0.0f, 0, 0, 0);
  //     constraintRef->setDamping(false);
  //     constraintRef->setAngularOnly(true);

  // #else
  //     gero::physics::PhysicSixDofConstraintDef constraintDef;
  //     constraintDef.body_a = bodyA;
  //     constraintDef.body_b = bodyB;
  //     // constraintDef.center = k_anchor;
  //     constraintDef.center = glm::vec3(0,0,0);

  //     auto constraintRef = this->physic.world->getPhysicSixDofConstraintManager().createAndAdd(constraintDef);

  //     constraintRef->setLinearLowerLimit(glm::vec3(0,0,0));
  //     constraintRef->setLinearUpperLimit(glm::vec3(0,0,0));
  //     // constraintRef->setLinearLowerLimit(k_anchor - glm::vec3(2));
  //     // constraintRef->setLinearUpperLimit(k_anchor + glm::vec3(2));

  //     // constraintRef->setAngularLowerLimit(-glm::vec3(gero::math::pi2, gero::math::pi2, gero::math::pi2));
  //     // constraintRef->setAngularUpperLimit(+glm::vec3(gero::math::pi2, gero::math::pi2, gero::math::pi2));
  //     // constraintRef->setAngularLowerLimit(-glm::vec3(0, 0, 0));
  //     // constraintRef->setAngularUpperLimit(+glm::vec3(0, 0, 0));

  //     // constraintRef->setLowerLimit(gero::math::pi * -0.6f, gero::math::hpi * -0.5f);
  //     // constraintRef->setUpperLimit(gero::math::hpi * +0.1f, gero::math::hpi * +0.1f);
  // #endif

  //   }
}
