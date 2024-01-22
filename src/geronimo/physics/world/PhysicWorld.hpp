
#pragma once

#include "../AbstractPhysicWorld.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include "../body/AbstractPhysicBodyManager.hpp"
#include "../vehicle/AbstractPhysicVehicleManager.hpp"

#include "../constraints/hinge/AbstractPhysicHingeConstraintManager.hpp"
#include "../constraints/six-dof/AbstractPhysicSixDofConstraintManager.hpp"

// not ready (-_-)
// #include "constraints/universal/AbstractPhysicUniversalConstraintManager.hpp"
// #include "constraints/cone-twist/AbstractPhysicConeTwistConstraintManager.hpp"

#include "../queries/query-shape/QueryShape.hpp"
#include "../queries/ray-caster/RayCaster.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btIDebugDraw;

class PhysicBodyManager;
class PhysicVehicleManager;
class PhysicHingeConstraintManager;
class PhysicUniversalConstraintManager;
class PhysicSixDofConstraintManager;
class PhysicConeTwistConstraintManager;

class btCollisionShape;
class btDefaultMotionState;
class btRigidBody;

namespace gero {
namespace physics {

class PhysicWorld : public AbstractPhysicWorld {

  friend PhysicBodyManager;
  friend PhysicVehicleManager;
  friend PhysicHingeConstraintManager;
  friend PhysicUniversalConstraintManager;
  friend PhysicSixDofConstraintManager;
  friend PhysicConeTwistConstraintManager;
  friend QueryShape;
  friend RayCaster;

public:
  // typedef std::vector<glm::vec3> t_vertices;
  // typedef std::vector<int32_t> t_indices;

  struct PhysicWorldOptions {
    std::size_t bodies_pre_allocated_size = 256;
    std::size_t vehicles_pre_allocated_size = 256;
    std::size_t hinge_pre_allocated_size = 256;
    std::size_t six_dof_pre_allocated_size = 256;
  };

  using debuggerPushLineCallback = std::function<void(const glm::vec3&, const glm::vec3&, const glm::vec3&)>;

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

  glm::vec3 _gravity;

public:
  PhysicWorld(std::optional<PhysicWorldOptions> options = std::nullopt);
  ~PhysicWorld();

  //
  //
  // world

public:
  void setDebuggerPushLine(const debuggerPushLineCallback& callback) override;

  void setDebugAABB(bool isEnabled) override;
  void setDebugContacts(bool isEnabled) override;
  void setDebugConstraints(bool isEnabled) override;
  void setDebugConstraintLimits(bool isEnabled) override;

public:
  void setGravity(float inX, float inY, float inZ) override;
  void setGravity(const glm::vec3& inGravity) override;
  const glm::vec3& getGravity() const override;

public:
  void step(float elapsedTime, uint32_t maxSubSteps, float fixedTimeStep) override;
  void renderDebug() override;

  //
  //
  // bodies

private:
  std::unique_ptr<AbstractPhysicBodyManager> _physicBodyManager;

public:
  AbstractPhysicBodyManager& getPhysicBodyManager() override;
  const AbstractPhysicBodyManager& getPhysicBodyManager() const override;

  //
  //
  // vehicles

private:
  std::unique_ptr<AbstractPhysicVehicleManager> _physicVehicleManager;

public:
  AbstractPhysicVehicleManager& getPhysicVehicleManager() override;
  const AbstractPhysicVehicleManager& getPhysicVehicleManager() const override;

  //
  //
  // constraints

private:
  std::unique_ptr<AbstractPhysicHingeConstraintManager> _physicHingeConstraintManager;

public:
  AbstractPhysicHingeConstraintManager& getPhysicHingeConstraintManager() override;
  const AbstractPhysicHingeConstraintManager& getPhysicHingeConstraintManager() const override;

  // not ready (-_-)

  // private:
  //   std::unique_ptr<AbstractPhysicUniversalConstraintManager> _physicUniversalConstraintManager;
  // public:
  //   AbstractPhysicUniversalConstraintManager& getPhysicUniversalConstraintManager();
  //   const AbstractPhysicUniversalConstraintManager& getPhysicUniversalConstraintManager() const;

private:
  std::unique_ptr<AbstractPhysicSixDofConstraintManager> _physicSixDofConstraintManager;

public:
  AbstractPhysicSixDofConstraintManager& getPhysicSixDofConstraintManager() override;
  const AbstractPhysicSixDofConstraintManager& getPhysicSixDofConstraintManager() const override;

  // private:
  //   std::unique_ptr<AbstractPhysicConeTwistConstraintManager> _physicConeTwistConstraintManager;
  // public:
  //   AbstractPhysicConeTwistConstraintManager& getPhysicConeTwistConstraintManager();
  //   const AbstractPhysicConeTwistConstraintManager& getPhysicConeTwistConstraintManager() const;

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
  // rayCast

private:
  RayCaster _rayCaster;

public:
  RayCaster& getRayCaster() override;

private:
  QueryShape _queryShape;

public:
  QueryShape& getQueryShape() override;

protected:
  btDiscreteDynamicsWorld* getRawDynamicsWorld() override;
};

} // namespace physics
} // namespace gero
