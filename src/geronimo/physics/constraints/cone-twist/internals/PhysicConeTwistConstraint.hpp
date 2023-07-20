
#pragma once

#include "../AbstractPhysicConeTwistConstraint.hpp"
#include "../PhysicConeTwistConstraintDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btConeTwistConstraint;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicConeTwistConstraintManager;

class PhysicConeTwistConstraint : public AbstractPhysicConeTwistConstraint {
  friend PhysicWorld;
  friend PhysicConeTwistConstraintManager;

private:
  struct Bullet {
    btConeTwistConstraint* constraint = nullptr;
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
  void setAngularOnly(bool angularOnly) override;
  bool getAngularOnly() const override;
  void setLimit(float _swingSpan1,
                float _swingSpan2,
                float _twistSpan,
                float _softness = 1.f,
                float _biasFactor = 0.3f,
                float _relaxationFactor = 1.0f) override;
  float getDamping() const override;
  void setDamping(float damping) override;

  // void	setAngularOnly(bool angularOnly);
  // bool    getAngularOnly() const;
  // void	setLimit(btScalar _swingSpan1,btScalar _swingSpan2,btScalar _twistSpan, btScalar _softness = 1.f, btScalar
  // _biasFactor = 0.3f, btScalar _relaxationFactor = 1.0f)

  // btScalar getDamping() const { return m_damping; }
  // void setDamping(btScalar damping) { m_damping = damping; }
};

using PhysicConeTwistConstraintsPool =
  safe_weak_ref_data_pool<PhysicConeTwistConstraint, AbstractPhysicConeTwistConstraint, 256, false>;

} // namespace physics
} // namespace gero
