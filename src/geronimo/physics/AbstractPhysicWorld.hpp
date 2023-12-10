
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include "body/AbstractPhysicBodyManager.hpp"
#include "vehicle/AbstractPhysicVehicleManager.hpp"

#include "constraints/hinge/AbstractPhysicHingeConstraintManager.hpp"
#include "constraints/six-dof/AbstractPhysicSixDofConstraintManager.hpp"

// not ready (-_-)
// #include "constraints/universal/AbstractPhysicUniversalConstraintManager.hpp"
// #include "constraints/cone-twist/AbstractPhysicConeTwistConstraintManager.hpp"

#include "queries/query-shape/QueryShape.hpp"
#include "queries/ray-caster/RayCaster.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

class btDiscreteDynamicsWorld;

class PhysicBodyManager;
class PhysicVehicleManager;
class PhysicHingeConstraintManager;
class PhysicUniversalConstraintManager;
class PhysicSixDofConstraintManager;
class PhysicConeTwistConstraintManager;

namespace gero {
namespace physics {

class AbstractPhysicWorld {

  friend PhysicBodyManager;
  friend PhysicVehicleManager;
  friend PhysicHingeConstraintManager;
  friend PhysicUniversalConstraintManager;
  friend PhysicSixDofConstraintManager;
  friend PhysicConeTwistConstraintManager;
  friend QueryShape;
  friend RayCaster;

public:
  struct PhysicWorldOptions {
    std::size_t bodies_pre_allocated_size = 256;
    std::size_t vehicles_pre_allocated_size = 256;
    std::size_t hinge_pre_allocated_size = 256;
    std::size_t six_dof_pre_allocated_size = 256;
  };

  using debuggerPushLineCallback = std::function<void(const glm::vec3&, const glm::vec3&, const glm::vec3&)>;

public:
  virtual ~AbstractPhysicWorld() = default;

  //
  //
  // world

public:
  virtual void setDebuggerPushLine(const debuggerPushLineCallback& callback) = 0;

  virtual void setDebugAABB(bool isEnabled) = 0;
  virtual void setDebugContacts(bool isEnabled) = 0;
  virtual void setDebugConstraints(bool isEnabled) = 0;
  virtual void setDebugConstraintLimits(bool isEnabled) = 0;

public:
  virtual void setGravity(float inX, float inY, float inZ) = 0;
  virtual void setGravity(const glm::vec3& inGravity) = 0;
  virtual const glm::vec3& getGravity() const = 0;

public:
  virtual void step(float elapsedTime, uint32_t maxSubSteps, float fixedTimeStep) = 0;
  virtual void renderDebug() = 0;

  //
  //
  // bodies

public:
  virtual AbstractPhysicBodyManager& getPhysicBodyManager() = 0;
  virtual const AbstractPhysicBodyManager& getPhysicBodyManager() const = 0;

  //
  //
  // vehicles

public:
  virtual AbstractPhysicVehicleManager& getPhysicVehicleManager() = 0;
  virtual const AbstractPhysicVehicleManager& getPhysicVehicleManager() const = 0;

  //
  //
  // constraints

public:
  virtual AbstractPhysicHingeConstraintManager& getPhysicHingeConstraintManager() = 0;
  virtual const AbstractPhysicHingeConstraintManager& getPhysicHingeConstraintManager() const = 0;

public:
  virtual AbstractPhysicSixDofConstraintManager& getPhysicSixDofConstraintManager() = 0;
  virtual const AbstractPhysicSixDofConstraintManager& getPhysicSixDofConstraintManager() const = 0;

  //
  //
  // rayCast

public:
  virtual RayCaster& getRayCaster() = 0;

public:
  virtual QueryShape& getQueryShape() = 0;

protected:
  virtual btDiscreteDynamicsWorld* getRawDynamicsWorld() = 0;

public:
  static std::unique_ptr<AbstractPhysicWorld> create();

};

} // namespace physics
} // namespace gero
