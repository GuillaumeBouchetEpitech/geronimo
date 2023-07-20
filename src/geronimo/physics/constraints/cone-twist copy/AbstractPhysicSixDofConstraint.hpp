
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/containers/weak_ref_data_pool.hpp"

namespace gero {
namespace physics {

class AbstractPhysicConeTwistConstraint : public gero::weak_ref_data_pool_base_class {
public:
  AbstractPhysicConeTwistConstraint() = default;
  virtual ~AbstractPhysicConeTwistConstraint() = default;
  AbstractPhysicConeTwistConstraint(const AbstractPhysicConeTwistConstraint& other) = delete;
  AbstractPhysicConeTwistConstraint(AbstractPhysicConeTwistConstraint&& other) = default;
  AbstractPhysicConeTwistConstraint& operator=(const AbstractPhysicConeTwistConstraint& other) = delete;
  AbstractPhysicConeTwistConstraint& operator=(AbstractPhysicConeTwistConstraint&& other) = default;

public:
	virtual void setLinearLowerLimit(const glm::vec3& linearLower) = 0;
	virtual void setLinearUpperLimit(const glm::vec3& linearUpper) = 0;
	virtual glm::vec3 getLinearLowerLimit() const = 0;
	virtual glm::vec3 getLinearUpperLimit() const = 0;

	virtual void setAngularLowerLimit(const glm::vec3& angularLower) = 0;
	virtual void setAngularUpperLimit(const glm::vec3& angularUpper) = 0;
	virtual glm::vec3 getAngularLowerLimit() const = 0;
	virtual glm::vec3 getAngularUpperLimit() const = 0;


};

using PhysicConeTwistConstraintWeakRef = data_pool_weak_ref<AbstractPhysicConeTwistConstraint>;

} // namespace physics
} // namespace gero
