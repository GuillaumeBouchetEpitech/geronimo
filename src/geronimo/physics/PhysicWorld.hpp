
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include "body/PhysicBodyManager.hpp"
#include "vehicle/PhysicVehicleManager.hpp"

#include "queries/QueryShape.hpp"
#include "queries/Raycaster.hpp"

#include <cstdint>
#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btIDebugDraw;

class PhysicBody;
class PhysicVehicle;

// class PhysicHingeConstraint;
// class PhysicFixedConstraint;

class btCollisionShape;
class btDefaultMotionState;
class btRigidBody;

namespace gero {
namespace physics {

class PhysicWorld {
  // private:
  //   static PhysicWorld* self;

  friend PhysicBodyManager;
  friend PhysicVehicleManager;
  friend QueryShape;
  friend Raycaster;

public:
  typedef std::vector<glm::vec3> t_vertices;
  typedef std::vector<int32_t> t_indices;

  using debuggerPushLineCallback =
    std::function<void(const glm::vec3&, const glm::vec3&, const glm::vec3&)>;

private:
  struct bullet {
    btBroadphaseInterface* broadphase = nullptr;
    btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
    btCollisionDispatcher* dispatcher = nullptr;
    btSequentialImpulseConstraintSolver* solver = nullptr;
    btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
  } _bullet;

  // struct ContactPair
  // {
  //   PhysicBody* bodyA = nullptr;
  //   PhysicBody* bodyB = nullptr;

  //   std::vector<ContactPoint*> contactsA;
  //   std::vector<ContactPoint*> contactsB;
  // };
  // std::unordered_map<void*, ContactPair> _contactMap;

public:
  PhysicWorld();
  ~PhysicWorld();

  //
  //
  // world

public:
  void setDebuggerPushLine(const debuggerPushLineCallback& callback);

public:
  void step(float elapsedTime, uint32_t maxSubSteps, float fixedTimeStep);
  void render();

  //
  //
  // bodies

private:
  PhysicBodyManager _physicBodyManager;

public:
  PhysicBodyManager& getPhysicBodyManager();
  const PhysicBodyManager& getPhysicBodyManager() const;

  //
  //
  // vehicles

private:
  PhysicVehicleManager _physicVehicleManager;

public:
  PhysicVehicleManager& getPhysicVehicleManager();
  const PhysicVehicleManager& getPhysicVehicleManager() const;

  //
  //
  // constraints

  // private:
  //   std::vector<PhysicHingeConstraint*> _hinge_constraints;
  // public:
  //   PhysicHingeConstraint* addHingeConstraint(const PhysicHingeConstraintDef&
  //   def); void destroyHingeConstraint(PhysicHingeConstraint* constraint);

  // private:
  //   std::vector<PhysicFixedConstraint*> _fixed_constraints;
  // public:
  //   PhysicFixedConstraint* addFixedConstraint(const PhysicFixedConstraintDef&
  //   def); void destroyFixedConstraint(PhysicFixedConstraint* constraint);

  //
  //
  // raycast

private:
  Raycaster _raycaster;

public:
  Raycaster& getRaycaster();

private:
  QueryShape _queryShape;

public:
  QueryShape& getQueryShape();
};

} // namespace physics
} // namespace gero
