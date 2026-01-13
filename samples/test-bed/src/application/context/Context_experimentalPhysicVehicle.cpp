
#if 1

#include "Context.hpp"

#include "geronimo/system/math/constants.hpp"

void Context::experimentalPhysicVehicle() {

  {
    gero::physics::PhysicBodyDef bodyDef;

    // const glm::vec3 chassisSize = { 1.0f, 2.0f, 0.5f };
    const glm::vec3 chassisSize = {2.0f, 4.0f, 1.0f};
    const glm::vec3 chassisHSize = chassisSize * 0.5f;

    bodyDef.shape.type = gero::physics::PhysicShapeDef::Type::compound;
    {
      gero::physics::PhysicShapeDef::Data::Compound::ChildShape childShape;
      {
        childShape.transform = glm::identity<glm::mat4>();
        childShape.transform = glm::translate(childShape.transform, glm::vec3(0.0f, 0.0f, 0.5f));
      }
      {
        childShape.shape = std::make_shared<gero::physics::PhysicShapeDef>();
        childShape.shape->type = gero::physics::PhysicShapeDef::Type::box;
        childShape.shape->data.box.size = {chassisSize.x * 0.5f, chassisSize.y, chassisSize.z};
      }
      bodyDef.shape.data.compound.childShapes.push_back(childShape);
    }
    {
      gero::physics::PhysicShapeDef::Data::Compound::ChildShape childShape;
      {
        childShape.transform = glm::identity<glm::mat4>();
        childShape.transform = glm::translate(childShape.transform, glm::vec3(0.0f, 0.0f, 0.5f));
      }
      {
        childShape.shape = std::make_shared<gero::physics::PhysicShapeDef>();
        childShape.shape->type = gero::physics::PhysicShapeDef::Type::box;
        childShape.shape->data.box.size = {chassisSize.x, chassisSize.y * 0.3f, chassisSize.z};
      }
      bodyDef.shape.data.compound.childShapes.push_back(childShape);
    }
    {
      {
        gero::physics::PhysicShapeDef::Data::Compound::ChildShape childShape;
        {
          childShape.transform = glm::identity<glm::mat4>();
          childShape.transform = glm::translate(childShape.transform, glm::vec3(0.0f, chassisSize.y * 0.3f, 0.25f));
        }
        {
          childShape.shape = std::make_shared<gero::physics::PhysicShapeDef>();
          childShape.shape->type = gero::physics::PhysicShapeDef::Type::box;
          childShape.shape->data.box.size = {chassisSize.x, 0.25f, 0.25f};
        }
        bodyDef.shape.data.compound.childShapes.push_back(childShape);
      }
      {
        gero::physics::PhysicShapeDef::Data::Compound::ChildShape childShape;
        {
          childShape.transform = glm::identity<glm::mat4>();
          childShape.transform = glm::translate(childShape.transform, glm::vec3(0.0f, chassisSize.y * -0.3f, 0.25f));
        }
        {
          childShape.shape = std::make_shared<gero::physics::PhysicShapeDef>();
          childShape.shape->type = gero::physics::PhysicShapeDef::Type::box;
          childShape.shape->data.box.size = {chassisSize.x, 0.25f, 0.25f};
        }
        bodyDef.shape.data.compound.childShapes.push_back(childShape);
      }
    }
    {
      gero::physics::PhysicShapeDef::Data::Compound::ChildShape childShape;
      {
        childShape.transform = glm::identity<glm::mat4>();
        childShape.transform = glm::translate(childShape.transform, glm::vec3(0.0f, 0.0f, 1.5f));
        childShape.transform = glm::rotate(childShape.transform, gero::math::qpi, glm::vec3(0, 1, 1));
      }
      {
        childShape.shape = std::make_shared<gero::physics::PhysicShapeDef>();
        childShape.shape->type = gero::physics::PhysicShapeDef::Type::box;
        childShape.shape->data.box.size = {0.75f, 0.75f, 0.75f};
      }
      bodyDef.shape.data.compound.childShapes.push_back(childShape);
    }

    bodyDef.mass = 0.1f;
    bodyDef.group = gero::asValue(Groups::player);
    bodyDef.mask = gero::asValue(Groups::ground);

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(bodyDef);

    body->setPosition({-10, -10, 5});
    body->setFriction(0.5f);
    body->disableSleep();

    //
    //

    const glm::vec3 wheelDirectionCS0 = {0.0f, 0.0f, -1.0f}; // down axis: -Z (toward the ground)
    const glm::vec3 wheelAxleCS = {1.0f, 0.0f, 0.0f};        // rotation axis: +X
    const float wheelRadius = 0.75f;
    const float wheelWidth = 0.3f;
    const float wheelSide = wheelWidth * 0.3f;

    // The maximum length of the suspension (metres)
    const float suspensionRestLength = 0.3f;

    // The maximum distance the suspension can be compressed (centimetres)
    const float maxSuspensionTravelCm = 20.0f; // <= 0.2 metres

    // The coefficient of friction between the tyre and the ground.
    // Should be about 0.8 for realistic cars, but can increased for better handling.
    // Set large (10000.0) for kart racers
    const float wheelFriction = 10000.0f; // <= "kart racer"

    // The stiffness constant for the suspension.
    // => 10.0: "Offroad buggy"
    // => 50.0: "Sports car"
    // => 200.0: "F1 Car"
    const float suspensionStiffness = 30.0f; // <= "Sports/F1 Car"

    // The damping coefficient for when the suspension is compressed.
    // Set to k * 2.0 * btSqrt(m_suspensionStiffness) so k is proportional to critical damping.
    // k = 0.0 undamped & bouncy, k = 1.0 critical damping
    // 0.1 to 0.3 are good values
    const float wheelsDampingCompression = 0.3f;

    // The damping coefficient for when the suspension is expanding.
    // See the comments for m_wheelsDampingCompression for how to set k.
    // m_wheelsDampingRelaxation should be slightly larger than m_wheelsDampingCompression, eg 0.2 to 0.5
    const float wheelsDampingRelaxation = 0.5f;

    // Reduces the rolling torque applied from the wheels that cause the vehicle to roll over.
    // This is a bit of a hack, but it's quite effective. 0.0 = no roll, 1.0 = physical behaviour.
    // If m_frictionSlip is too high, you'll need to reduce this to stop the vehicle rolling over.
    // You should also try lowering the vehicle's centre of mass
    const float rollInfluence = 0.15f;

    gero::physics::PhysicVehicleDef vehicleDef;
    vehicleDef.body = body;
    vehicleDef.coordinateSystem = {0, 2, 1};
    vehicleDef.wheelsCollisionGroup = gero::asValue(Groups::player);
    vehicleDef.wheelsCollisionMask = gero::asValue(Groups::ground);
    vehicleDef.allWheelStats.reserve(4);

    struct WheelData {
      glm::vec3 connectionPoint;
      bool isFrontWheel;
    };

    std::array<WheelData, 4> wheels{
      {// front right
       {{+chassisHSize.x - wheelSide, +chassisHSize.y - wheelRadius, 0.5f}, /*isFrontWheel = */ true},
       // front left
       {{-chassisHSize.x + wheelSide, +chassisHSize.y - wheelRadius, 0.5f}, /*isFrontWheel = */ true},
       // rear right
       {{+chassisHSize.x - wheelSide, -chassisHSize.y + wheelRadius, 0.5f}, /*isFrontWheel = */ false},
       // rear left
       {{-chassisHSize.x + wheelSide, -chassisHSize.y + wheelRadius, 0.5f}, /*isFrontWheel = */ false}}};

    for (auto& wheel : wheels) {
      gero::physics::PhysicVehicleDef::WheelStats wheelStats;

      wheelStats.connectionPoint = wheel.connectionPoint;
      wheelStats.isFrontWheel = wheel.isFrontWheel;
      wheelStats.maxSuspensionTravelCm = maxSuspensionTravelCm;
      wheelStats.rollInfluence = rollInfluence;
      wheelStats.suspensionRestLength = suspensionRestLength;
      wheelStats.suspensionStiffness = suspensionStiffness;
      wheelStats.wheelAxleCS = wheelAxleCS;
      wheelStats.wheelDirectionCS0 = wheelDirectionCS0;
      wheelStats.wheelFriction = wheelFriction;
      wheelStats.wheelRadius = wheelRadius;
      wheelStats.wheelsDampingCompression = wheelsDampingCompression;
      wheelStats.wheelsDampingRelaxation = wheelsDampingRelaxation;

      vehicleDef.allWheelStats.push_back(wheelStats);
    }

    auto vehicle = this->physic.world->getPhysicVehicleManager().createAndAddVehicle(vehicleDef);

    // for (int ii = 0; ii < 4; ++ii)
    //   vehicle->applyEngineForce(ii, 0.25f);

    // front wheels
    for (int ii = 0; ii < 2; ++ii) {
      vehicle->setSteeringValue(ii, gero::math::opi);
      vehicle->applyEngineForce(ii, 0.25f);
    }

    body->setPosition({80, -40, 30});

    glm::quat orientation = glm::identity<glm::quat>();
    // 180 flip so it "face down"
    orientation *= glm::angleAxis(gero::math::pi, glm::normalize(glm::vec3(1, 0, 0)));

    // custom orientation -> to show it "falling" and "recover"
    orientation *= glm::angleAxis(gero::math::qpi, glm::normalize(glm::vec3(1, 1, 0)));

    body->setOrientation(orientation);

    // custom gravity -> "fall upward"
    body->setGravity(0, 0, +10);
  }

  { // vehicle floor

    {

      gero::physics::PhysicBodyDef def;
      def.shape.type = gero::physics::PhysicShapeDef::Type::box;
      // def.shape.data.box.size = {1, 50, 50};
      def.shape.data.box.size = {50, 50, 1};
      def.mass = 0.0f;

      auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

      body->setPosition({80, -40, 40});
    }
  }
}

#endif
