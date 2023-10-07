
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/containers/weak_ref_data_pool.hpp"

namespace gero {
namespace physics {

class AbstractPhysicSixDofConstraint : public gero::weak_ref_data_pool_base_class {
public:
  AbstractPhysicSixDofConstraint() = default;
  virtual ~AbstractPhysicSixDofConstraint() = default;
  AbstractPhysicSixDofConstraint(const AbstractPhysicSixDofConstraint& other) = delete;
  AbstractPhysicSixDofConstraint(AbstractPhysicSixDofConstraint&& other) = default;
  AbstractPhysicSixDofConstraint& operator=(const AbstractPhysicSixDofConstraint& other) = delete;
  AbstractPhysicSixDofConstraint& operator=(AbstractPhysicSixDofConstraint&& other) = default;

public:
  virtual void setLinearLowerLimit(const glm::vec3& linearLower) = 0;
  virtual void setLinearUpperLimit(const glm::vec3& linearUpper) = 0;
  virtual glm::vec3 getLinearLowerLimit() const = 0;
  virtual glm::vec3 getLinearUpperLimit() const = 0;

  virtual void setAngularLowerLimit(const glm::vec3& angularLower) = 0;
  virtual void setAngularUpperLimit(const glm::vec3& angularUpper) = 0;
  virtual glm::vec3 getAngularLowerLimit() const = 0;
  virtual glm::vec3 getAngularUpperLimit() const = 0;

  virtual void setLinearBounce(float x, float y, float z) = 0;
  virtual void setAngularBounce(float x, float y, float z) = 0;

  virtual void setLinearSpring(bool x, bool y, bool z) = 0;
  virtual void setAngularSpring(bool x, bool y, bool z) = 0;

  // ERP, error reduction parameter, is a rate of how fast the error is reduced
  virtual void setLinearStopERP(float x, float y, float z) = 0;
  // ERP, error reduction parameter, is a rate of how fast the error is reduced
  virtual void setAngularStopERP(float x, float y, float z) = 0;

  // CFM, constraint force mixing, adds some small value to the main diagonal of the constraint matrix, to avoid degenerate matrices
  virtual void setLinearStopCFM(float x, float y, float z) = 0;
  // CFM, constraint force mixing, adds some small value to the main diagonal of the constraint matrix, to avoid degenerate matrices
  virtual void setAngularStopCFM(float x, float y, float z) = 0;


};

using SixDofConstraintWeakRef = data_pool_weak_ref<AbstractPhysicSixDofConstraint>;

} // namespace physics
} // namespace gero
