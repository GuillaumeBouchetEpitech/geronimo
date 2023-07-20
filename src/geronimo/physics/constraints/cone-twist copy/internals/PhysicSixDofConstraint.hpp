
#pragma once

#include "../AbstractPhysicConeTwistConstraint.hpp"
#include "../PhysicConeTwistConstraintDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btGeneric6DofSpring2Constraint;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicConeTwistConstraintManager;

class PhysicConeTwistConstraint : public AbstractPhysicConeTwistConstraint {
  friend PhysicWorld;
  friend PhysicConeTwistConstraintManager;

private:
  struct Bullet {
    btGeneric6DofSpring2Constraint* constraint = nullptr;
  } _bullet;

  BodyWeakRef _body;

  bool _isAdded = false;

public:
  PhysicConeTwistConstraint(const PhysicConeTwistConstraintDef& def);
  virtual ~PhysicConeTwistConstraint();

  PhysicConeTwistConstraint(const PhysicConeTwistConstraint& other) = delete;
  PhysicConeTwistConstraint(PhysicConeTwistConstraint&& other);
  PhysicConeTwistConstraint& operator=(const PhysicConeTwistConstraint& other) = delete;
  PhysicConeTwistConstraint& operator=(PhysicConeTwistConstraint&& other);

public:
	void setLinearLowerLimit(const glm::vec3& linearLower) override;
	void setLinearUpperLimit(const glm::vec3& linearUpper) override;
	glm::vec3 getLinearLowerLimit() const override;
	glm::vec3 getLinearUpperLimit() const override;

	void setAngularLowerLimit(const glm::vec3& angularLower) override;
	void setAngularUpperLimit(const glm::vec3& angularUpper) override;
	glm::vec3 getAngularLowerLimit() const override;
	glm::vec3 getAngularUpperLimit() const override;

};

using PhysicConeTwistConstraintsPool = safe_weak_ref_data_pool<PhysicConeTwistConstraint, AbstractPhysicConeTwistConstraint, 256, false>;

} // namespace physics
} // namespace gero
