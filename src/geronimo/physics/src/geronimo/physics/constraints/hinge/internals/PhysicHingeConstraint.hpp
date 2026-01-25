
#pragma once

#include "../AbstractPhysicHingeConstraint.hpp"
#include "../PhysicHingeConstraintDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btDiscreteDynamicsWorld;
class btHingeConstraint;
// class btHinge2Constraint;

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
    // btHinge2Constraint* constraint = nullptr;
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

public:
  void setLimit(float low, float high) override;
};

using PhysicHingeConstraintsPool =
  safe_weak_ref_data_pool<PhysicHingeConstraint, AbstractPhysicHingeConstraint, 256, false>;

} // namespace physics
} // namespace gero
