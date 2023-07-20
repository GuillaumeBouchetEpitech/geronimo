
#pragma once

#include "../AbstractPhysicHingeConstraint.hpp"
#include "../PhysicHingeConstraintDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btDiscreteDynamicsWorld;
class btHingeConstraint;


namespace gero {
namespace physics {

class PhysicWorld;
class PhysicHingeConstraintManager;

class PhysicHingeConstraint : public AbstractPhysicHingeConstraint {
  friend PhysicWorld;
  friend PhysicHingeConstraintManager;

private:
  struct Bullet {
    btHingeConstraint* constraint = nullptr;
  } _bullet;

  BodyWeakRef _body;

  bool _isAdded = false;

public:
  PhysicHingeConstraint(const PhysicHingeConstraintDef& def);
  virtual ~PhysicHingeConstraint();

  PhysicHingeConstraint(const PhysicHingeConstraint& other) = delete;
  PhysicHingeConstraint(PhysicHingeConstraint&& other);
  PhysicHingeConstraint& operator=(const PhysicHingeConstraint& other) = delete;
  PhysicHingeConstraint& operator=(PhysicHingeConstraint&& other);

// public:
//   void applyEngineForce(int32_t index, float force) override;
//   void applyBrake(int32_t index, float force) override;
//   void setSteeringValue(int32_t index, float steering) override;
//   void reset() override;

// public:
//   int32_t getNumWheels() const override;
//   const glm::mat4& getWheelTransform(int32_t index, glm::mat4& mat4x4) const override;
//   glm::vec3 getWheelPosition(int32_t index) const override;
//   glm::quat getWheelOrientation(int32_t index) const override;
//   float getCurrentSpeedKmHour() const override;
//   BodyWeakRef getPhysicBody() override;
//   const BodyWeakRef getPhysicBody() const override;
};

using PhysicHingeConstraintsPool = safe_weak_ref_data_pool<PhysicHingeConstraint, AbstractPhysicHingeConstraint, 256, false>;

} // namespace physics
} // namespace gero
