
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
  virtual void setAngularOnly(bool angularOnly) = 0;
  virtual bool getAngularOnly() const = 0;
  virtual void setLimit(float _swingSpan1,
                        float _swingSpan2,
                        float _twistSpan,
                        float _softness = 1.f,
                        float _biasFactor = 0.3f,
                        float _relaxationFactor = 1.0f) = 0;

  virtual float getDamping() const = 0;
  virtual void setDamping(float damping) = 0;
};

using PhysicConeTwistConstraintWeakRef = data_pool_weak_ref<AbstractPhysicConeTwistConstraint>;

} // namespace physics
} // namespace gero
