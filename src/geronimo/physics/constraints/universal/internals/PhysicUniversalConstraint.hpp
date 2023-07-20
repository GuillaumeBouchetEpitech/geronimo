
#pragma once

#include "../AbstractPhysicUniversalConstraint.hpp"
#include "../PhysicUniversalConstraintDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btUniversalConstraint;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicUniversalConstraintManager;

class PhysicUniversalConstraint : public AbstractPhysicUniversalConstraint {
  friend PhysicWorld;
  friend PhysicUniversalConstraintManager;

private:
  struct Bullet {
    btUniversalConstraint* constraint = nullptr;
  } _bullet;

  BodyWeakRef _body;

  bool _isAdded = false;

public:
  PhysicUniversalConstraint(const PhysicUniversalConstraintDef& def);
  virtual ~PhysicUniversalConstraint();

  PhysicUniversalConstraint(const PhysicUniversalConstraint& other) = delete;
  PhysicUniversalConstraint(PhysicUniversalConstraint&& other);
  PhysicUniversalConstraint& operator=(const PhysicUniversalConstraint& other) = delete;
  PhysicUniversalConstraint& operator=(PhysicUniversalConstraint&& other);

public:
  glm::vec3 getAnchor1() const override;
  glm::vec3 getAnchor2() const override;
  glm::vec3 getAxis1() const override;
  glm::vec3 getAxis2() const override;
  float getAngle1() const override;
  float getAngle2() const override;
  void setUpperLimit(float angle1Max, float angle2Max) override;
  void setLowerLimit(float angle1Min, float angle2Min) override;
};

using PhysicUniversalConstraintsPool =
  safe_weak_ref_data_pool<PhysicUniversalConstraint, AbstractPhysicUniversalConstraint, 256, false>;

} // namespace physics
} // namespace gero
