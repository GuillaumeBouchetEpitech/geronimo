
#pragma once

#include "../AbstractPhysicSixDofConstraint.hpp"
#include "../PhysicSixDofConstraintDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btGeneric6DofSpring2Constraint;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicSixDofConstraintManager;

class PhysicSixDofConstraint : public AbstractPhysicSixDofConstraint {
  friend PhysicWorld;
  friend PhysicSixDofConstraintManager;

private:
  struct Bullet {
    btGeneric6DofSpring2Constraint* constraint = nullptr;
  } _bullet;

  BodyWeakRef _body;

  bool _isAdded = false;

public:
  PhysicSixDofConstraint(const PhysicSixDofConstraintDef& def);
  virtual ~PhysicSixDofConstraint();

  PhysicSixDofConstraint(const PhysicSixDofConstraint& other) = delete;
  PhysicSixDofConstraint(PhysicSixDofConstraint&& other);
  PhysicSixDofConstraint& operator=(const PhysicSixDofConstraint& other) = delete;
  PhysicSixDofConstraint& operator=(PhysicSixDofConstraint&& other);

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

using PhysicSixDofConstraintsPool = safe_weak_ref_data_pool<PhysicSixDofConstraint, AbstractPhysicSixDofConstraint, 256, false>;

} // namespace physics
} // namespace gero
