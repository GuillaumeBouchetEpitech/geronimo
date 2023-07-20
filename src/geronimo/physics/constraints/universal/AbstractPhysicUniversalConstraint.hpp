
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/containers/weak_ref_data_pool.hpp"

namespace gero {
namespace physics {

class AbstractPhysicUniversalConstraint : public gero::weak_ref_data_pool_base_class {
public:
  AbstractPhysicUniversalConstraint() = default;
  virtual ~AbstractPhysicUniversalConstraint() = default;
  AbstractPhysicUniversalConstraint(const AbstractPhysicUniversalConstraint& other) = delete;
  AbstractPhysicUniversalConstraint(AbstractPhysicUniversalConstraint&& other) = default;
  AbstractPhysicUniversalConstraint& operator=(const AbstractPhysicUniversalConstraint& other) = delete;
  AbstractPhysicUniversalConstraint& operator=(AbstractPhysicUniversalConstraint&& other) = default;

public:
  virtual glm::vec3 getAnchor1() const = 0;
  virtual glm::vec3 getAnchor2() const = 0;
  virtual glm::vec3 getAxis1() const = 0;
  virtual glm::vec3 getAxis2() const = 0;
  virtual float getAngle1() const = 0;
  virtual float getAngle2() const = 0;
  virtual void setUpperLimit(float angle1Max, float angle2Max) = 0;
  virtual void setLowerLimit(float angle1Min, float angle2Min) = 0;

};

using UniversalConstraintWeakRef = data_pool_weak_ref<AbstractPhysicUniversalConstraint>;

} // namespace physics
} // namespace gero
